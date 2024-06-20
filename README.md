# SolarBoilerHeating
Simple Arduino sketch for controlling solar heating for the boiler.

It runs on the Wemos D1 board

## Connections
Relays connected to pins 12, 13

DS18B20 temp sensors on pin 14

SSD1306 display connected to I2C (SDA, SCL pins)

## Description
Measures water temperatures in boiler and solar panels with DS18B20 temperature sensors.

When the water temperature in solar panels gets above the threshold it switches the relay to turn on the heating.

Measured temperatures and heating state (ON or OFF) are displayed on the SSD1306 display.
