# 🪑 Intellichair
<img width="1097" height="543" alt="image" src="https://github.com/user-attachments/assets/ab02113c-0a97-4d13-b0fd-c7df51466992" />

## 🧩 Hardware Used
- Arduino Uno / Nano (microcontroller)

- 4 × Full Bridge Load Cells (weight sensors)

- 4 × HX711 Amplifier Modules (to read load cell signals)

- Buzzer Module (posture alert system)

## 🔧 Sensor Module (Load Cell + HX711)
The sensor module consists of a load cell paired with an HX711 amplifier.
- **Load Cell:** A strain gauge-based sensor that measures the force (weight) applied to it. Each corner of the chair has one load cell, allowing the system to calculate weight distribution.

- **HX711 Amplifier:** A precision 24-bit ADC (Analog-to-Digital Converter) specifically designed for weight scales. It converts the tiny electrical signals from the load cells into readable digital values for the Arduino.

## Research Summary
There are two pathways of quantification - symmetry based metrics and Center of Pressure (CoP) based metric.
### Symmetry Based Metrics

In biomechanics, **symmetry** refers to how similar the left and right sides of the body in regard to gait (i.e. during movement like walking, running etc.). *Perfect symmetry* means both sides behave identically while *perfect asymmetry* means one side is doing more/less work, which often points to injury, imbalance, or poor posture. There are several mathematical indices to quantify this symmetry[^1] :
- **Ratio Index (RI)**: The Ratio Index uses the ratio of the values for the two limbs as the index of symmetry. For a variable X, it is defined as $RI = (1 - \frac{X_{nd}}{X_{d}}) * 100$ where $X_{nd}$ represents the non dominant limb and $X_{d}$ is the dominant one. RI = 0% indicates full symmetry, while RI > 100% indicates asymmetry. Values can exceed 100% and are not bounded. A negative value is possible if $X_{d} < X_{nd}$. 
- **Gait Index (GI)**: The Gait Asymmetry index is simply a logarithmic transform of the ratio index. It is defined as $GA = ln(X_{d}/X_{nd}) * 100$. Again, GA = 0% indicates full symmetry while GA > 100% indicates full asymmetry. Values can exceed 100% and are not bounded. A negative value is possible if $X_d < X_{nd}$. Note that the GA can only be defined when the ratio between $X_d$ and $X_{nd}$ is positive and hence is not easily used when the variable to be measured can take both positive and negative values (flexion and extension in typical movement assessments, for instance).
- **Symmetry Index (SI)**: The Symmetry Index is the most commonly used index and is a generalization of the RI, where the denominator is taken to be the average of the absolute values for both limbs. **There are alternative definitions where either limb is taken as the reference**. In such cases, the SI simply reduces to the RI. The standard SI is defined as $SI = 2 \cdot \frac{X_{d} - X_{nd}}{|X_{d}| + |X_{nd}|}\cdot 100$ . Again, SI = 0% indicates full symmetry, whereas SI > 100% indicates full asymmetry. As in the above, values can exceed 100% and are not bounded. A negative value indicates that $X_{d} < X_{nd}$. Because of this, in some applications, the absolute value of the SI is reported rather than the SI itself.
- **Symmetry Angle (SA)**: The Symmetry Angle is a fairly newly developed index that captures symmetry in angular data. It is defined as $SA = \frac{45^{o} - arctan(\frac{X_{nd}}{X_d})}{90^{o}} \cdot 100$ . SA = 0% indicates full symmetry and SA > 100% indicates full asymmetry. 

Another mathematical model worth mentioning is the Limb Loading Error (LLE), which provides a loading error individual to each limb[^2] :
                 **Limb Loading Error (LLE) in %** = $[\frac{O1}{P1} - 1] \cdot 100$ 
where, O1 is the observed loading and P1 is the prescribed loading, which has been set to half of the body weight for the healthy population. 

### Center of Pressure (CoP) Metric

The **center of pressure (CoP)** is a key concept in biomechanics and postural control studies, and it’s exactly what researchers use the **Nintendo Wii Balance Board** for. If the subject is sitting or standing on a surface with **4 load sensors**, each sensor measures a portion of the subject's total weight. The **CoP** is the **weighted average point of pressure** where the net vertical force acts. Therefore, if the **subject leans left, the CoP leans left**, if the **subject leans right, the CoP leans right** and if the **subject sits perfectly balanced, the CoP stays in the middle**. 

![1-s2 0-S0966636213003184-gr1_lrg](https://github.com/user-attachments/assets/ca44076f-3be6-4d2a-a1db-6418d3d795f3)

Based on the schematic image of the Wii board, the following calculations suggest the formulae used for the CoP[^3]:
                     $CoP_x  = \frac{L}{2}\frac{(TR + BR) - (TL + BL)}{TR + BR + TL + BL}$
                     $CoP\_y = \frac{L}{2}\frac{(TR + TL) - (BR + BL)}{TR + BR + TL + BL}$

wherein $CoP_x$ determines Left/Right balance (horizontal shift) and $CoP_y$ determines Forward/Backward balance (vertical shift).
## Relevance to our Project

In terms of the symmetry indices, most of the indices and similar measures we see in biomechanics papers are developed for **gait analysis** (limb movement while walking/running). They assume alternating steps and cyclical motion. But the **concept of symmetry** (comparing left vs. right side loads/forces) is general — researchers have also applied the same idea to **seated posture, balance on force plates, and pressure-mapping studies**. Therefore, it is completely relevant if we adapt these metrics to our chair design.

For the CoP metric, this aligns perfectly with Intellichair because Wii board is basically the same structure we are attempting to replicate, but with a slightly different application and purpose. 

## The Right Choices

If we follow the symmetry pathway, since our Intellichair is intended for old and/or disabled individuals, therefore, the LLE isn't supposed to be a proper metric of measurement, considering the ambiguity with P1. In addition, SA incorporates a complex trigonometry formula which might be unfamiliar to clinicians[^2].  The limitation of the RI is its low sensitivity and relatively small asymmetry value[^2]. However, although SI gives an inflated asymmetry value, utilizing it is of relevance because of its conventional nature and the popularity.

Technically speaking, CoP is an equally important right choice, in light of the fact that Nintendo has successfully utilized this and thus we have a production ready system.

## Challenges

Both **SI (Symmetry Index)** and **CoP (Center of Pressure)** methods face the _threshold problem_, i.e. to what extend the inclination is allowed.
- **SI** → Ambiguous because:
	-  A person with scoliosis or asymmetrical muscle strength may have a "natural" asymmetry.
    -  What looks like imbalance for one person may be their baseline.
- **CoP** → Ambiguous because:
	-  Different body sizes → same CoP shift means different real postures.
	-  Seat size matters (5 cm shift on a 30 cm seat is big, but on a 60 cm seat it’s small).
	-  People wiggle naturally — how much deviation is actually “bad”?





[^1]: Queen, Robin, et al. "A novel method for measuring asymmetry in kinematic and kinetic variables: the normalized symmetry index." Journal of biomechanics 99 (2020): 109531.

[^2]: 2. Kumar, Senthil NS, et al. "Evaluation of limb load asymmetry using two new mathematical models." Global Journal of Health Science 7.2 (2014): 1.

[^3]: 3. Bartlett, Harrison L., Lena H. Ting, and Jeffrey T. Bingham. "Accuracy of force and center of pressure measures of the Wii Balance Board." Gait & posture 39.1 (2014): 224-228.
