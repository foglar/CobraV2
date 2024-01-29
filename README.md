# CobraV2

## Installation and compilation

```bash
git clone https://www.github.com/foglar/cobraV2.git
cd cobraV2/serial_read

# Building serial read code yourself
go build main.go
```

Upload sender and reciever code on the 2 arduino's

## Overview

`reciever_module/` - folder with code for reciver which will send data to the pc
`sender_module/` - folder with code for sender, which transmit data to the reciever and save it on the micro sd card
`serial_read/` - read serial input and save it

## Modules

### 10 DOF IMU

- **SDA** (Serial Data)
- **SCL** (Serial Clock)
- **INT** (Interrupt) is a digital interrupt output
- **FSYNC** is a frame sync signal

### L76K GPS

- **PPS** (Pulse per second) powers a led wich will blink and indicate that GPS is working
- **RX** (UART communication)
- **TX** (UART communication)

#### NMEA Sentence

$GPGAA,HHMMSS.SS,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx

| Sentence Identifier | Time | Latitude | Longtitude | Fix Quality | Number of Satellites | Horizontal Dilution of Precision | Altitude | Height of geoid above WGS84 ellipsoid | Time since last DGPS update | DGPS reference station id | Checksum |
| ------ | --------- | --------- | ---------- | - | -- | --- | ----- | ----- | --- | ---- | --- |
| $GPGAA | HHMMSS.SS | llll.ll a | yyyyy.yy a | x | xx | x.x | x.x M | x.x M | x.x | xxxx | *hh |
| $GPGAA | 170834 | 4124.8963, N | 08151.6838, W | 1 | 05 | 1.5 | 280.2, M | -34.0, M | blank | blank | *75 |

Fix Quality:

- 0 = Invalid
- 1 = GPS fix
- 2 = DGPS fix

[more about nmea sentences](https://aprs.gids.nl/nmea/)

### NRF24L01+

- **CE** (Chip Enable) is an active-high pin. When enabled, the nRF24L01 will either transmit or receive, depending on the mode.
- **CSN** (Chip Select Not) is an active-low pin that is typically held HIGH. When this pin goes low, the nRF24L01 begins listening for data on its SPI port and processes it accordingly.
- **SCK** (Serial Clock) accepts clock pulses from the SPI bus master.
- **MOSI** (Master Out Slave In) is the SPI input for the nRF24L01.
- **MISO** (Master In Slave Out) is the SPI output of the nRF24L01.
- **IRQ** is an interrupt pin that can notify the master when there is new data to process.

## Wiring

### 10 DOF IMU Sensor

| Module | Arduino | Description    |
| ------ | ------- | -------------- |
| VCC    | 5V      | Power input    |
| GND    | GND     | Power ground   |
| SDA    | SDA     | I2C data input |
| SCL    | SCL     | I2C clock pin  |

### L76K GPS Module

| Module | Arduino | Description          |
| ------ | ------- | -------------------- |
| VCC    | 3.3V    | Power input          |
| GND    | GND     | Power ground         |
| TX     | RX      | Serial communication |
| RX     | TX      | Serial communication |
| PPS    | NC      | Pulse per second     |

### NRF24L01+ Module

| Module | Arduino | Description     |
| ------ | ------- | --------------- |
| VCC    | 3.3V    | Power input     |
| GND    | GND     | Power ground    |
| CE     | 9~      | Chip enable     |
| CSN    | 8       | Chip select not |
| SCK    | 13      | Serial Clock    |
| MOSI   | 11~     | SPI master out  |
| MISO   | 12      | SPI master in   |

## Sources

Datasheets, documentation and sources

- [Waveshare 10 DOF IMU Sensor (D) - Arduino guide](https://www.waveshare.com/wiki/Template:10_DOF_IMU_Sensor_D_Arduino_Guide)
- [Waveshare 10 DOF IMU Sensor (C) - Demo code](https://files.waveshare.com/upload/c/c1/10_DOF_IMU_Sensor_D_Code.7z)
- [Waveshare 10 DOF IMU Sensor (C) - Documentation](https://www.waveshare.com/wiki/10_DOF_IMU_Sensor_(C))
- [Waveshare L76K GPS Module - Arduino guide](https://www.waveshare.com/wiki/L76K_GPS_Module#Working_With_Arduino)
- [Waveshare L76K GPS Module - Demo code](https://files.waveshare.com/upload/9/94/L76K_GPS_Module_Arduino.zip)
- [NRF24L01+ PA/LNA - Arduino guide](https://lastminuteengineers.com/nrf24l01-arduino-wireless-communication/)
- [NRF24L01+ Guide - Guide](https://navody.dratek.cz/navody-k-produktum/arduino-wifi-modul-nrf24l01.html)
- [NRF24L01+ PA/LNA - Datasheet](nrf24lo1-datasheet.pdf)
- [NRF24L01+ PA/LNA - Demo code](https://img.gme.cz/files/eshop_data/eshop_data/10/775-034/prg.775-034.1.zip)
