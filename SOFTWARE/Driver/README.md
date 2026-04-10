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


## TIMER
CH1/CH1n 

### PWM

### INTERUPT SOURCE

## OPAMP

### PGA MODE 

## ADC

### DMA and RANK

## GPIO

### BASIC LOGIC

## LPHUART


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

