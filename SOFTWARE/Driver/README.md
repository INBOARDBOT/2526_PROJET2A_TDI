# 2526 · PROJET 2A · TDI
> **Total Drive Immersion** — Système de plateforme à motion

---

## Présentation

Ce dépôt contient le firmware du **driver** de la plateforme TDI. Le driver est le maillon qui traduit les consignes reçues depuis la centrale en commandes concrètes pour faire bouger la plateforme. Il tourne sur un microcontrôleur STM32 et gère l'ensemble de la chaîne : mesure des capteurs, contrôle moteur FOC, communication et supervision système.

---

## Architecture du code

```
.
├── Drivers/              # Librairies bas-niveau (ARM Math DSP, HAL STM32)
└── Core/
    ├── inc/              # Fichiers d'en-tête générés
    ├── src/              # Fichiers source générés
    ├── startup/          # Fichiers de démarrage générés
    └── lib/
        ├── application/  # Couche applicative (centrale, pyGraph, supervision globale)
        ├── filters/      # Filtres pour l'estimation correcte des capteurs
        ├── FOC/          # Contrôle moteur (Field-Oriented Control)
        ├── peripherals/  # Configuration et pilotage des capteurs
        ├── PID/          # Correcteurs PID
        └── test/         # Fichiers de test des différents sous-systèmes
```

---

## Configuration matérielle (IOC)

### Cahier des charges du MCU

| Fonction | Description |
|---|---|
| Mesure de courant | Sur les trois phases moteur via résistances shunt |
| Mesure de tension bus | Vbus sur ADC1 rang 1 |
| Position angulaire | Encodeur incrémental en quadrature (A, B, Z) |
| Communication | LPUART via ST-LINK (prog / debug / release) |
| Contrôle moteur | PWM triphasé (BLDC) |

---

## Timers

### TIM1 — Synchronisation principale
Timer haute priorité, cœur du système temps réel.

- **CH1/CH1n, CH2/CH2n, CH3/CH3n** : signaux PWM triphasés envoyés au moteur BLDC.
- **CH4** (sans sortie physique) : référence de synchronisation — déclenche la mesure de courant sur l'injecteur ADC via *toggle on match*.

La fréquence PWM est configurable directement via les registres `ARR` et `PSC`.

### TIM2 — Correcteur vitesse *(à implémenter)*
Timer logiciel cadencé à **500 Hz**, destiné au correcteur en vitesse. Non encore implémenté.

### TIM3 — Mode encodeur
Aucune horloge de référence interne. Le compteur est incrémenté par l'encodeur externe raccordé en quadrature (fronts montants et descendants sur les signaux **A** et **B**). Le signal **Z** déclenche une interruption GPIO à chaque révolution complète.

---

## Sources d'interruption (ISR)

| Fonction | Déclencheur | Rôle |
|---|---|---|
| `HAL_GPIO_EXTI_Callback()` | Signal Z encodeur | Détection d'une révolution complète |
| `HAL_TIM_PeriodElapsedCallback()` | Débordement timer | Appel périodique selon la config du timer |
| `HAL_ADCEx_InjectedConvCpltCallback()` | TIM1 CH4 toggle | Mesure de tension aux bornes de la résistance shunt |

---

## Chaîne de mesure du courant

### 1 · OPAMP (amplification)
Les OPAMPs internes au MCU amplifient la tension aux bornes des résistances shunt. Cette tension étant très faible, elle ne serait pas correctement résolue par l'ADC sur sa plage [0 V, 3,3 V] sans amplification.

Mode utilisé : **PGA à connexion interne** — gain configurable en puissance de 2, montage différentiel avec rebouclage sur l'entrée inverseuse, sortie simultanément lisible par l'ADC.

### 2 · ADC (conversion)
- Résolution : **12 bits**
- Cycle de traitement : **6,5 cycles** (compatible avec un délai d'interruption de 100 µs)
- Déclenchement : `TIM1 CH4 toggle on match`
- Lecture : directement depuis les registres ADC

| ADC | Rang 1 | Rang 2 |
|---|---|---|
| `ADC1` | Mesure Vbus | Mesure Vshunt phase C |
| `ADC2` | Mesure Vshunt phase B | Mesure Vshunt phase A |

---

## GPIO

| Signal | Direction | Description |
|---|---|---|
| Adresse 2 bits | Entrée | Identifiant matériel du driver |
| Bouton utilisateur | Entrée | Interaction manuelle |
| LEDs (hors alimentation) | Sortie | Indicateurs d'état |
| Signal Z encodeur | Entrée + IRQ | Détection révolution complète |

---

## Communication

**LPUART** — liaison série basse consommation via le connecteur ST-LINK embarqué.
Utilisée pour la programmation, le débogage et la mise en production.

# Capteurs & Contrôleurs
> Documentation technique du firmware TDI — couche mesure et régulation

L'ensemble des capteurs sont configurés dans leurs propres fichiers, regroupés dans `peripherals/`.

---

## Mesure du courant

La mesure est effectuée toutes les **100 µs**, synchronisée sur la fin du signal PWM. À cet instant précis, une seule porte du demi-pont supérieur est fermée et deux portes du demi-pont inférieur sont fermées — c'est la fenêtre idéale pour une mesure propre, sans bruit de commutation.

On mesure directement **Ia** (en phase) et **Ib** (déphasé de π/3), puis **Ic** est déduit par la loi des nœuds :

```
Ic = -(Ia + Ib)
```

![Fenêtre d'échantillonnage ADC](img_src\adc_sample.png)

### Conversion tension → courant

La formule théorique est :

```
I = Vbus × valeur_ADC / ((2¹² - 1) × gain_PGA × gain_externe × R_shunt)
```

En pratique, les résultats obtenus avec cette formule étaient aberrants — les courants calculés évoquaient davantage un four à induction industriel qu'un moteur BLDC 24 V. Les valeurs de gain réelles du circuit ne correspondaient pas aux valeurs nominales.

**Solution adoptée :** calibration expérimentale. Des courants connus ont été injectés via un générateur de courant, et les valeurs ADC correspondantes ont été relevées. La relation linéaire obtenue sert de référence de conversion.

![Courbes de calibration courant/ADC](img_src\amp_mesaure.png)

L'exploitation de ces courbes est détaillée dans la section *Contrôleur courant*.

---

## Mesure de la position instantanée

L'encodeur incrémental compte les impulsions des signaux **A** et **B** en quadrature. À chaque révolution complète, le signal **Z** se déclenche et le compteur revient à 0. Le timer TIM3 est configuré avec `ARR = 4095` pour correspondre exactement à cette plage.

La conversion en radians est directe :

```
θ = valeur_mesurée / 4095 × 2π
```

### Mesure de la vitesse

La vitesse angulaire est calculée par différence de position sur une fenêtre temporelle fixe :

```
ω = Δθ / Δt       avec Δt = 2 ms  (cadence 500 Hz)
```

La cadence de 500 Hz est délibérément plus lente que la mesure de courant (10 kHz). À 10 kHz, la variation de position entre deux échantillons vaut systématiquement 1 pulse — la mesure serait saturée et inutilisable. À 500 Hz, la variation est suffisamment grande pour être significative tout en restant réactive.

---

## Mesure de la tension de bus

La conversion est simple car il n'y a pas de shunt ni d'amplification différentielle — la tension de bus est divisée par un pont résistif directement lisible par l'ADC :

```
V_bus = valeur_ADC / (2¹² - 1) × V_ref
```

---

## Contrôleurs

Les contrôleurs corrigent les écarts entre la consigne et la mesure réelle. Ils sont organisés en trois boucles imbriquées, du plus rapide au plus lent : **courant → vitesse → position**.

![Correcteur en cascade](img_src\cascade_controler.png)

---

## Contrôleur courant

C'est la boucle la plus rapide et la plus critique du système. Elle s'exécute à **10 kHz** (toutes les 100 µs) et garantit que le moteur ne surconsomme pas, quelle que soit la consigne de couple.

![Current Control](img_rsc\current_control.png)

### Transformées de Clarke et Park

Le moteur BLDC génère des grandeurs triphasées sinusoïdales (Ia, Ib, Ic) qui varient en permanence avec la position du rotor. Pour appliquer un correcteur simple et stable, on projette ces grandeurs dans un repère fixe puis tournant.

**Transformée de Clarke** (3 phases → 2 axes fixes α/β) :

```
Iα =  Ia
Iβ = (Ia + 2·Ib) / √3
```

Cette étape réduit le système triphasé à deux composantes orthogonales dans un repère stationnaire lié au stator.

**Transformée de Park** (axes fixes α/β → axes tournants d/q) :

```
Id =  Iα·cos(θ) + Iβ·sin(θ)
Iq = -Iα·sin(θ) + Iβ·cos(θ)
```

En se plaçant dans le repère lié au rotor (qui tourne à la même vitesse que le champ magnétique), **Id** et **Iq** deviennent des grandeurs continues en régime permanent. Id contrôle le flux (maintenu à 0 pour un BLDC), Iq contrôle le couple. On peut donc leur appliquer des correcteurs PI classiques.

### SVPWM — Space Vector PWM

Une fois les tensions de commande Vd et Vq calculées par les PI, on applique la transformée inverse de Park pour revenir en α/β, puis le **SVPWM** génère les rapports cycliques des trois phases.

Le SVPWM décompose le vecteur tension souhaité (Vα, Vβ) en une combinaison pondérée de deux vecteurs actifs adjacents parmi les six vecteurs de base de l'onduleur, complétée par des vecteurs nuls. L'avantage par rapport à un simple sinus de référence est une meilleure utilisation de la tension de bus (facteur ~1,15) et une symétrie des commutations qui réduit les harmoniques.

### Correcteur PI

Le correcteur utilisé est un **Proportionnel-Intégrateur** :

```
u(t) = Kp·e(t) + Ki·∫e(t)dt
```

- Le terme **proportionnel** assure la réactivité.
- Le terme **intégrateur** élimine l'erreur statique en régime permanent.
- Pas de terme **dérivateur** : une variation brusque du courant génère un pic d'erreur dérivée qui déstabilise le correcteur — la boucle courant est déjà suffisamment rapide sans lui.

---

## Contrôleur vitesse

> *En cours de développement.*

---

## Contrôleur position

> *En cours de développement.*

---

## Test en boucle ouverte

Le test en boucle ouverte permet de valider le bon fonctionnement des capteurs et de la chaîne de commande **avant** de fermer les boucles de régulation. C'est l'étape de vérification indispensable avant toute mise en régulation.

**Ce qu'on vérifie :**

| Signal | Comportement attendu |
|---|---|
| Position encodeur | Alignée avec la consigne angulaire imposée |
| Courant mesuré | Forme d'onde caractéristique SVPWM (créneaux modulés) |

Si l'encodeur ne suit pas la consigne ou si le courant présente des distorsions anormales, le problème vient de la configuration matérielle (polarité shunt, sens encodeur, offset ADC) et doit être corrigé avant d'activer les correcteurs.

# Resources 
Documentation STM32G431VBT [link1](https://www.st.com/en/evaluation-tools/32f746gdiscovery.html#st_description_sec-nav-tab) <br>
Documentation OPAMPs [link2](https://support.touchgfx.com/docs/introduction/welcome)<br>
Documentation ADCs [link3](https://arm-software.github.io/CMSIS_5/RTOS/html/cmsis__os_8h.html)<br>
Documentation schema EVLSPIN32G4-ACT [link4](https://www.st.com/resource/en/schematic_pack/evlspin32g4-act-schematics.pdf)<br>
ARM math library [link5](https://arm-software.github.io/CMSIS_5/DSP/html/index.html)<br>
HAL libraries [link6](https://sourcevu.sysprogs.com/stm32/HAL/symbols/HAL_ADCEx_InjectedStart_IT)<br>
The goat SIROJU [link7](https://github.com/sirojudinMunir/stm32-FOC)<br>

