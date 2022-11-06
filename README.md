# Embedded Systems - Ventilation Project - Group 5


_Daniel Radvanyi, Manish Subedi, Melany Macias, Sophia Schwalb_<br>
Metropolia University of Applied Science IT engineering students, Embedded Systems major. 


## Preface

The ventilation system consisted of a **lpc1549 microcontroller**, a **Produal MIO 12V actuator**, sensors from 
Vaisala and Sesirion, and a fan. 
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

### LPC1549 

LPC1549, an ARM-cortex M3 based microcontroller was used in this project, that features a rich peripheral set with very low power consumption.
The LPC1549 operate at CPU frequencies of up to 72 MHz. The LPC1549 include up to 256 kB of flash memory, 32 kB of ROM, a 4 kB EEPROM, and 
up to 36 kB of SRAM. 
Among the features, mainly, NVIC, Systick Timer, Watchdog timer, communication protocols: I2C, Modbus, and UART, were used for this project. 

### GMP252 co2 probe

Vaisala's GMP252 co2 probe was used to measure and collect data on the co2 concentration. GMP252 is designed for CO2 measurement in demanding applications that require reliable and accurate performance. The measurement range is 0 - 10 000 ppmCO2 (measurements can be carried out in the 
10 000 - 30 000 ppmCO2 range with reduced accuracy). It is an intelligent, stand-alone, ppm-level probe. It's intended for measuring CO2 in agriculture, refrigeration, greenhouses, demanding HVAC applications, and for plant growth chamber manufacturers.
[source: https://www.vaisala.com/en/products/instruments-sensors-and-other-measurement-devices/instruments-industrial-measurements/gmp252]

### HMP60

Another one from Vaisala, HMP60 Humidity and Temperature Probe measured and transmitted temperature and humidity readings. HMP60 series probes use the interchangeable Vaisala INTERCAP® sensor. No recalibration is required after sensor replacement. It is a simple, durable and cost-effective humidity probe suitable for volume applications, integration into other manufacturers' equipment, incubators, glove boxes, greenhouses, fermentation chambers and data loggers.
[source: https://www.vaisala.com/en/products/instruments-sensors-and-other-measurement-devices/instruments-industrial-measurements/hmp60]

### SDP600 Series - Low-cost Digital Differential Pressure Sensor
SDP600 differential pressure sensor measured the pressure difference between inside and outside of the air duct. The SDP600 sensor family is Sensirion’s series of digital  differential pressure sensors designed for high-volume 
applications. They measure the pressure of air and non-aggressive gases with superb accuracy and no offset drift. 
The sensors cover a pressure range of up to ±500 Pa
(±2 inch H2O / ±5 mbar) and deliver outstanding accuracy 
even at the bottom end of the measuring range.
[source: https://sensirion.com/products/catalog/SDP600-25Pa/]

### Produal MIO 

It is the actuator that controlls the speed of the fan. The voltage range is 0 - 10V, meaning the fan is at maximum frequency/speed at 10V. 

## Networks and communication

The netowrk connectivity was configured on esp8266 esp01s that enables the board to be connected to the network. 

The **lpc board and the web server engine** both **publish and subscribe** to **MQTT** messages from _'Controller/Settings'_. 
But only the board publishes to the topic: _'Controller/Status'_. The Web UI displays data accordingly.

Vaisala's HMP60, _relative humidity and temperature sensor_, _(modbus address 241)_ the GMP252 co2 probe _(modbus address 240)_, both interact via **Modbus RTU protocol** with the lpc board. The fan is connected to the Produal MIO 12-V actuator which also uses modbus protocol for communication with the board _(modbus address 1)_.

The Sensirion's SDP610 pressure sensor interacts via I2C lines with the lpc board. The I2C address of the sensor is 0x40 and the _SCL is at port 0, pin 22_ on the lpc board wheras, _SDA lies at port 0, pin 23_. 

All the data is received by the program running on the lpc board and transmitted accordingly. Right now, the message is published approximately every **transmission time** 



