# 2526_PROJET2A_TDI
Total Drive Immersion - Motion System


The objective of this project is to create a 3 axis motion plateform for car racing simulations.
It consist of four linear actuators (with their respective drivers), a Nucleo Discovery which handle
the communication between the drivers and the computer as well as a touchscreen interface.
The data is sent from SimHub to the the Nucleo via UART. Then the position values are distributed to each actuator via an RS485 bus.

Master - Slave synchonization cycle  
![Diagram Name](masterslaveSync.drawio.svg)

Backend - Frontend synchonization cycle  
![Diagram Name](backendfrontendSync.drawio.svg)
