# Offline Vehicle-to-Vehicle Communication System

This project focuses on the development of an offline Vehicle-to-Vehicle (V2V) communication system, designed to facilitate data exchange between two vehicles. The system is primarily implemented on the Arduino platform, utilizing a variety of sensors and communication modules to achieve real-time, wireless communication without the need for an internet connection.

## Project Components

The following components and sensors are integral to the successful operation of the V2V communication system:

### 1. [Arduino Mega Boards](https://www.arduino.cc/en/Main/ArduinoBoardMega2560)

The [Arduino Mega 2560](https://store.arduino.cc/products/arduino-mega-2560-rev3) microcontroller boards serve as the central processing units for the project prototypes. These boards are based on the ATmega2560 microcontroller and provide the following features:

- **Digital Input/Output Pins:** 54 digital I/O pins, offering extensive connectivity options.
- **Analog Inputs:** 16 analog input pins for reading sensor data.
- **Hardware Serial Ports:** 4 hardware serial ports for robust communication.
- **Operating Voltage:** 5 volts.
- **Clock Speed:** 16 MHz.
- **Communication Protocols Supported:** I2C, SPI, and Serial communication protocols.
- **Compatibility:** Compatible with a wide range of sensor modules.

The Arduino Mega boards are utilized to interface with all sensors, manage data processing, and facilitate communication between the two vehicles.

### 2. [Ultrasonic Sensor](https://components101.com/sensors/ultrasonic-sensor-working-pinout-datasheet)

The [Ultrasonic Sensor](https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/) plays a critical role in distance measurement within the system. It operates as follows:

- **Operating Frequency:** Approximately 40 kHz.
- **Range:** Several meters, depending on environmental conditions.
- **Components:** Consists of a transmitter that emits ultrasonic pulses and a receiver that detects the reflected pulses.
- **Working Principle:** 
  - The sensor emits an ultrasonic pulse, which travels through the air until it encounters an object.
  - The pulse is reflected back to the sensor upon hitting the object.
  - The sensor calculates the time taken for the pulse to travel to the object and back, using the speed of sound in air to determine the distance.
- **Calculation Methods:** Distance is typically calculated using time-of-flight principles or pulse width modulation techniques.
- **Output:** The calculated distance is sent to the Arduino for further processing.

### 3. [NEO-6M GPS Module](https://www.u-blox.com/en/product/neo-6-series)

The [NEO-6M GPS module](https://lastminuteengineers.com/neo6m-gps-arduino-tutorial/) provides real-time geographic positioning data by receiving signals from GPS satellites:

- **Satellite Tracking:** Capable of tracking up to 22 satellites across 50 channels.
- **Antenna:** Includes a built-in antenna for capturing satellite signals and decoding them into usable data.
- **Data Output:** Provides information such as latitude, longitude, altitude, and time in the form of NMEA sentences.
- **Communication:** Interfaces with the Arduino via serial communication.
- **Usage:** The Arduino processes the NMEA sentences to extract relevant real-time data, such as the vehicle's speed.

### 4. [nRF24L Transceiver Module](https://components101.com/wireless/nrf24l01-pinout-features-datasheet)

The [nRF24L transceiver module](https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/) is essential for wireless communication between vehicles:

- **Operating Frequency:** Functions within the 2.4 GHz ISM band.
- **Communication Channels:** Supports 125 RF channels, allowing for multi-channel communication.
- **Data Rate:** Capable of transmitting data at rates up to 2 Mbps.
- **Range:** Effective up to 100 meters in open air.
- **Protocol:** Utilizes the Enhanced ShockBurst™ protocol, which enables efficient two-way data packet communication.
- **Network Architecture:** Establishes a peer-to-peer (P2P) network for data transmission.
- **Functionality:** The transceiver module sends and receives data packets formatted according to the protocol. The receiving Arduino processes the data for display or further actions.

### 5. [LCD with I2C Interface](https://learn.adafruit.com/i2c-spi-lcd-backpack/overview)

The [LCD module with I2C interface](https://www.electronicwings.com/arduino/lcd20x4-i2c-display) is used to display real-time data:

- **Display:** A 16x2 Blue LCD screen.
- **Communication Protocol:** Utilizes the I2C protocol for efficient communication with the Arduino.
- **Backlight:** Equipped with a backlight for enhanced visibility in various lighting conditions.
- **Functionality:** 
  - The Arduino sends data to the LCD module in byte format via the I2C bus.
  - The LCD module processes the received data and updates the display accordingly.
  - The display continuously refreshes to provide real-time outputs.
