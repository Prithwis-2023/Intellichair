// Libraries: install "HX711" by Bogdan Necula (bogde) via Library Manager.
#include <HX711.h>

constexpr byte SCK_PIN = 2;
constexpr byte DOUT_PINS[4] = {3, 4, 5, 6}; // FL, FR, RL, RR (order matters)
constexpr byte BUZZER_PIN = 9;

// Seating presence threshold (in kg after calibration)
constexpr float SEATED_MIN_WEIGHT = 10.0f;   // tweak for your use-case

// Imbalance thresholds (on [0,1])
constexpr float THRESH_LR  = 0.15f;          // 15% left-right asymmetry
constexpr float THRESH_FB  = 0.15f;          // 15% front-back asymmetry
constexpr float THRESH_COP = 0.20f;          // CoP distance threshold

// How long imbalance must persist (ms) before buzzing
constexpr unsigned long PERSIST_MS = 30000UL; // 30 s

// Simple smoothing (exponential moving average)
constexpr float ALPHA = 0.2f;                 // 0..1 (higher = less smoothing)

// HX711 scale factors (raw -> kg). Fill these after calibration.
float SCALE[4]  = { 1.00f, 1.00f, 1.00f, 1.00f }; // FL, FR, RL, RR
float OFFSET[4] = { 0.f,   0.f,   0.f,   0.f   }; // set by tare()

HX711 hx[4];

float wFilt[4] = {0,0,0,0};
unsigned long imbalanceStart = 0;
bool imbalanceActive = false;

void tareAll(unsigned avg = 15) {
  for (int i = 0; i < 4; ++i) {
    hx[i].set_scale(1.0);         // temporary scale = 1 during tare
    hx[i].tare(avg);
    OFFSET[i] = hx[i].get_offset();
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; ++i) {
    hx[i].begin(DOUT_PINS[i], SCK_PIN);
    hx[i].set_scale(SCALE[i]);
    hx[i].set_offset(OFFSET[i]);
  }
  pinMode(BUZZER_PIN, OUTPUT);
  tareAll();
  Serial.println("Smart Chair: 4x HX711 imbalance monitoring");
  Serial.println("Send 't' to tare; 'c' to run per-corner calibration wizard.");
}

bool readAllKg(float w[4], unsigned avg = 5) {
  // Returns false if any HX711 isn't ready
  for (int i = 0; i < 4; ++i) {
    if (!hx[i].is_ready()) return false;
  }
  for (int i = 0; i < 4; ++i) {
    // get_units uses current scale & offset; average 'avg' samples
    w[i] = hx[i].get_units(avg);  // expected unit: "kg" after you set SCALE[i]
    if (!isfinite(w[i])) return false;
    if (w[i] < 0) w[i] = 0;       // clamp tiny negatives
  }
  return true;
}

void buzzNow(unsigned freq = 2000, unsigned durMs = 500) {
  tone(BUZZER_PIN, freq, durMs);
}

void loop() {
  // Serial commands
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 't') {
      tareAll();
      Serial.println("Tare done.");
    } else if (c == 'c') {
      runCalibrationWizard();
    }
  }

  float w[4];
  if (!readAllKg(w, 4)) return;

  // Smooth each corner
  for (int i = 0; i < 4; ++i) {
    wFilt[i] = ALPHA * w[i] + (1 - ALPHA) * wFilt[i];
  }

  // Total weight & seated detection
  float W = wFilt[0] + wFilt[1] + wFilt[2] + wFilt[3];
  bool seated = (W >= SEATED_MIN_WEIGHT);

  // Ratios only meaningful when seated
  float I_LR = 0.f, I_FB = 0.f, I_CoP = 0.f, x = 0.f, y = 0.f;
  if (seated) {
    float L = wFilt[0] + wFilt[2]; // FL + RL
    float R = wFilt[1] + wFilt[3]; // FR + RR
    float F = wFilt[0] + wFilt[1]; // FL + FR
    float B = wFilt[2] + wFilt[3]; // RL + RR

    I_LR = fabsf(L - R) / W;
    I_FB = fabsf(F - B) / W;

    x = R / W; // rightward ratio
    y = B / W; // backward ratio
    I_CoP = sqrtf((x - 0.5f)*(x - 0.5f) + (y - 0.5f)*(y - 0.5f)) / sqrtf(0.5f);
  } else {
    // reset if nobody is seated
    imbalanceStart = 0;
    imbalanceActive = false;
  }

  // Detect sustained imbalance
  bool over = seated && (I_LR > THRESH_LR || I_FB > THRESH_FB || I_CoP > THRESH_COP);
  unsigned long now = millis();
  if (over) {
    if (!imbalanceActive) { imbalanceActive = true; imbalanceStart = now; }
    if (now - imbalanceStart >= PERSIST_MS) {
      buzzNow();                 // alert!
      imbalanceStart = now + 2000; // rate-limit beeps (~every 2.5 s when still over)
    }
  } else {
    imbalanceActive = false;
    imbalanceStart = 0;
  }

  // Telemetry
  static unsigned long lastPrint = 0;
  if (now - lastPrint >= 200) {
    lastPrint = now;
    Serial.print("kg FL,FR,RL,RR: ");
    for (int i = 0; i < 4; ++i) { Serial.print(wFilt[i], 2); Serial.print(i<3? ", ":""); }
    Serial.print(" | W: "); Serial.print(W, 2);
    Serial.print(" | I_LR: "); Serial.print(I_LR, 3);
    Serial.print(" I_FB: "); Serial.print(I_FB, 3);
    Serial.print(" I_CoP: "); Serial.print(I_CoP, 3);
    Serial.print(" | x: "); Serial.print(x, 3);
    Serial.print(" y: "); Serial.println(y, 3);
  }
}

// --- Simple per-corner calibration wizard ---
// Place a known weight on ONE corner at a time when prompted.
void runCalibrationWizard() {
  Serial.println("\n=== Calibration Wizard ===");
  Serial.println("Make sure nobody is on the chair. Press ENTER to tare.");
  waitEnter_();
  tareAll();
  Serial.println("Tared. We'll compute SCALE[i] in kg/reading.");

  float known;
  Serial.println("Type the known calibration weight in kg (e.g., 5.0), then ENTER:");
  known = waitFloat_();

  const char* names[4] = {"FL","FR","RL","RR"};
  for (int i = 0; i < 4; ++i) {
    Serial.print("Place "); Serial.print(known,3); Serial.print(" kg on "); Serial.print(names[i]);
    Serial.println(" corner ONLY, then press ENTER.");
    waitEnter_();

    // average raw units (temporarily set scale=1, offset already set)
    hx[i].set_scale(1.0);
    long raw = 0;
    const int N = 20;
    for (int k = 0; k < N; ++k) raw += hx[i].read_average(10);
    float rawAvg = (float)raw / N - hx[i].get_offset();

    SCALE[i] = rawAvg / known; // units per kg
    hx[i].set_scale(SCALE[i]);
    Serial.print("Computed SCALE["); Serial.print(names[i]); Serial.print("] = ");
    Serial.println(SCALE[i], 2);
  }

  Serial.println("Calibration done. Consider saving SCALE[] constants into the sketch.");
}

void waitEnter_() {
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') break;
    }
  }
}

float waitFloat_() {
  String s;
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') {
        if (s.length() > 0) return s.toFloat();
      } else {
        s += c;
      }
    }
  }
}
