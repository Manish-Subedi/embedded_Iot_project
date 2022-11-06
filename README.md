# Embedded Systems - Ventilation Project - Group 5


_Daniel Radvanyi, Manish Subedi, Melany Macias, Sophia Schwalb_
Metropolia University of Applied Science IT engineering students, Embedded Systems major. 

## Preface

A ventilation system was controlled with a **lpc1549 microcontroller** and a **Produal MIO 12V actuator**
with data collected from the connected sensors.
Sensors were constantly transmitting temperature, relative humidity, co2 concentration, pressure, and
the speed of the fan which regulated the pressure level on the duct. MQTT was the choice of protocol that 
acted as the bridge for communication between Web UI and the system control. A local LCD UI also existed that 
facilitated on-spot easy controlling of the system. The functional features of both the UIs are quite alike in terms
of controlling the system. However, the Web UI is feature-rich in numerous ways.

While the entirety of this document is focused on the ***embedded part*** of the project. The details and technicalities
on ***web part***
can be found under the following link:

https://github.com/nessa01macias/Ventilation-Project


## Operation Guide

The system consists of two modes that are discussed below. 
Both modes are available on both of the interfaces: Web and LCD (local). 

While there exists a slider _(to adjust the set value)_ in web UI _(and a button to publish the set value to MQTT topic)_, the LCD UI can be controlled with the three buttons on the lpc board.  
The button closest to the LCD navigates up the menu items, the next one is to select,
and the next one navigates down the menu items. The buttons are configured with GPIO interrupts. 

The modes discussed below can be selected from both of the interfaces. It is easy to switch between the modes with a toggle button on the Web UI. Then slide the slider left or right. 
The LCD UI allows setting the modes in its first menu item. So switch between modes by pressing ok (select button) while the modes is visible on the display and then switch to desired mode with either up or down button. As we have only two options, it does not matter which button is used to navigate between modes. 
Follow the link above for Web UI specific queries. 

### Automatic mode

User can controll the pressure inside the duct either by setting pressure from the Web UI or locally on the LCD UI.
In case of Web UI, the set value is then published to a MQTT topic that the **lpc** is subscribing to. As per the set value,
the lpc tries to adjust the speed of the fan which tends to meet the desired pressure level eventually. The value set on LCD
is directly read and the program tries to achieve the set level pressure. Pressure level ranges from 0 - 120 pa. 

### Manual mode

User can controll the speed of the fan either by setting the speed from the Web UI or locally on the LCD UI.
In case of Web UI, the set value is then published to a MQTT topic that the **lpc** is subscribing to. As per the set value,
the lpc tries to adjust the speed of the fan. The value set on LCD is directly read and the program sets the speed. 
Speed can be set from 0 - 100%. 


## Microcontroller and Sensors 

LPC1549, an ARM-cortex M3 based microcontroller was used in this project. This microcontroller offers extensive features among which NVIC,
Systick Timer, Watchdog timer, communication protocols: I2C, Modbus, and UART were exploited for the project, together with Produal MIO 12V actuator. 

Vaisala's GMP252 co2 probe was used to measure and collect data on the co2 concentration. Another one from Vaisala,
HMP60 measured and transmitted temperature and humidity readings. Sensirion's SDP600 differential pressure sensor
measured the pressure difference between inside and outside of the air duct. 

## Networks and communication
The netowrk connectivity is configured on esp8266 socket server that enables the board to be connect to the network. 
The lpc and the web server engine both published and subscribes to messages from _'Controller/Status'_
----

## On this page
{:.no_toc}

- TOC
{:toc}

----

## Modules and Controllers

