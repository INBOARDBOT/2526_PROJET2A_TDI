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

# Capteur

## Mesure de la position instantanee


## Mesure du courrant

## Mesure de la tension du bus

## Test en boucle ouverte

### Encoder

### ADC

# Controleur

## Controleur courrant

### Presentation

### PARK-CLARKE

### SVPWM

### PI controler

## Controleur vitesse
not done

## Controleur position
not done


# Resources 
Documentation STM32G431VBT [link1](https://www.st.com/en/evaluation-tools/32f746gdiscovery.html#st_description_sec-nav-tab) <br>
Documentation OPAMPs [link2](https://support.touchgfx.com/docs/introduction/welcome)<br>
Documentation ADCs [link3](https://arm-software.github.io/CMSIS_5/RTOS/html/cmsis__os_8h.html)<br>
Documentation schema EVLSPIN32G4-ACT [link4](https://www.st.com/resource/en/schematic_pack/evlspin32g4-act-schematics.pdf)<br>
ARM math library [link5](https://arm-software.github.io/CMSIS_5/DSP/html/index.html)<br>
HAL libraries [link6](https://sourcevu.sysprogs.com/stm32/HAL/symbols/HAL_ADCEx_InjectedStart_IT)<br>
The goat SIROJU [link7](https://github.com/sirojudinMunir/stm32-FOC)<br>

