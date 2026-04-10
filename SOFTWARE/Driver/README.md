# 2526_PROJET2A_TDI
Total Drive Immersion - Motion System


# DRIVER CONTROL
Le driver est la partie du systeme qui relie les consignes de la centrale et fait bouger la plateforme.  

# Architecture du code
Le code est construit de la maniere suivante :
- Drivers : librairies ARM math DSP et HAL
- Core    : 
   - inc, src, startup generated files (at the exception of main.c)
   - lib : 
      - application layer : high level, control the communication with the centrale and external tools (pyGraph), control the global system
      - filters : filter used for sensor correct estimation
      - FOC : control of the motor using different control method
      - peripherals : all the sensor control and configuration
      - PID : self explanatory
      - test : test file for different pat of the driver system


# Setup IOC

## Cahier de charge
Le MCU du driver doit pouvoir faire les choses suivantes:
- Mesure de courant
- Mesure de la tension du bus
- Mesure de la position angulaire exacte
- Communication uart
- Controle du moteur en PWM triphase
- rapidite d'execution


## TIMER
Les timers sont utilises pour synchroniser l'execution du systeme, les prises de mesure et les calculs.

# TIM1
Le timer 1 est un timer qui doit synchroniser les prises de mesure en courant sur les trois phases, ainsi que le controle du moteur sur trois phases.

CH1/CH1n, CH2/CH2n et CH3/CH3n sont les signaux PWM envoyes sur le moteur.
CH4 est un channel sans sortie, c'est un signal de reference qui permet de synchroniser le moment ou une prise de mesure de courant doit etre effectuee sur l'injecteur de l'ADC (nous aurons l'occasion d'en reparler). Le plus important est de comprendre qu'il permet de declencher la mesure de courant sur le moteur.

# TIM2
Le timer 2 est un timer moins hardware, il contient le correcteur en vitesse a 500 Hz qui n'a pas encore ete implemente.

# TIM3
Le timer 3 est en mode encodeur, il ne prends aucune clock de reference. Le timer est incremente par un encodeur externe relie en quadrature (signaux A, B et Z). Un coup d'horloge est effectue quand un pas sur l'encodeur a ete effectue (A et B en rising edge puis falling edge).

### PWM
Comme explique precedemment nous utilsons des signaux PWM pour controller le moteur BLDC 3-phases, la frequence peut etre directement modifie en accedant au registre du timer ARR et PSC. 

### INTERUPT SOURCE
L'interupt ou ISR est un signal hardware controle par le NVIC, il va entre autre arrete la boucle d'execution principale pour execute du code contenue dans une fonction d'interupt. Voici une liste des fonctions interupt que nous utilisons :
- HAL_GPIO_EXTI_Callback() : interupt des GPIOs, permet de physiquement reperer une revolution complete sur lencodeur (signal Z).
- HAL_TIM_PeriodElapsedCallback() : interupt des timers, est appele periodiquement selon la configuration du timer.
- HAL_ADCEx_InjectedConvCpltCallback() : interupt des ADCs, est appelle quand TIM1CH4 change d'etat. Prends une mesure de la tension au bord de la Vshunt.

## OPAMP
Les OPAMPs ou AOP sont les composants internes du MCU qui permet de mesurer la tension au bord de la resistance de shunt avec un gain constant. Son utilite est d'amplifier le signal d'entre pour que l'ADC puisse faire une mesure exact, etant donne que la tension au bord de resistance de shunt est tres faible l'ADC va avoir du mal a mesure une tension sur sa plage de tension [0V, 3.3V]. La configuration des OPAMPs permet de determiner comment le circuit est implemente sur le MCU, le mode utilise est le PGA internally connected qui permet de regler un gain voulu (en puissance de 2) tout en permettant au circuit de faire un differentiel, cad laisser la sortie reboucler sur l'entree inverseuse et en meme temps pouvoir lire la sortie sur l'ADC.


## ADC
Les adc sont l'etape apres les OPAMPs, ils prennent une mesure sur le declenchement de TIM1CH4 "toggle on match". Leur resolution est de 12bits avec un cycle de traitement de 6.5cycles qui correspond au temps d'execution max pour avoir une mesure sur un delai d'interupt de 100us.


### DMA and RANK
Pour recuperer les valeurs des ADCs nous prenons directement les valeurs sur les registres des ADCs. 
ADC1 comprends rank 1 (mesure Vbus), rank 2 (mesure vshunt C)
ADC2 comprends rank 1 (mesure vshunt B), rank 2 (mesure vshunt A)


## GPIO
We have different GPIO input/output

The 2-bit address is a GPIO input
The user button il also a GPIO input
The different leds (besides alimentation) are output
The Z signal from the encoder is on a GPIO input with interupt


## LPHUART
This is the low power ST-LINK used for prog/debug/release

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

