# CobraV2

## Table of contents

- [CobraV2](#cobrav2)
  - [Table of contents](#table-of-contents)
  - [Installation and compilation](#installation-and-compilation)
    - [Serial monitor tool](#serial-monitor-tool)
    - [Monitoring tool](#monitoring-tool)
    - [Sender and Reciever](#sender-and-reciever)
  - [Overview](#overview)
    - [Schema](#schema)
      - [Sender](#sender)
      - [Reciever](#reciever)
      - [Master](#master)
    - [Arduino Sender Format](#arduino-sender-format)
  - [Modules](#modules)
    - [10 DOF IMU](#10-dof-imu)
    - [L76K GPS](#l76k-gps)
      - [NMEA Sentence](#nmea-sentence)
    - [NRF24L01+](#nrf24l01)
  - [Wiring](#wiring)
    - [10 DOF IMU Sensor](#10-dof-imu-sensor)
    - [L76K GPS Module](#l76k-gps-module)
    - [NRF24L01+ Module](#nrf24l01-module)
  - [Issues / features](#issues--features)
    - [Sender issues](#sender-issues)
    - [Monitor app issues](#monitor-app-issues)
  - [Sources](#sources)

## Installation and compilation

### Serial monitor tool

```shell
git clone https://www.github.com/foglar/cobraV2.git
cd cobraV2/serial_read

go run .
go build .
```

> [!TIP]
> You may have to delete `go.mod` file (if you have other go version) and `go mod init` the project.
> To install all dependencies you can use `go mod tidy` command or you can mannualy install them by `go get [package]` command.

### Monitoring tool

```shell
git clone https://www.github.com/foglar/cobraV2.git
cd cobraV2/monitor

go run .
go build .
```

> [!IMPORTANT]
> For cross-compiling you will need to specify a `CC` to compile c-based libraries for pixel2 library.
> Example command: `CC=x86_64-w64-mingw32-gcc GOOS=windows GOARCH=amd64 CGO_ENABLED=1 go build .`
> For more information see: [Pixel2 Wiki | Cross Compiling](https://github.com/gopxl/pixel/wiki/%5BWIP%5D-Cross-Compiling)

### Sender and Reciever

Upload sender and reciever code on the 2 arduinos

Required library for antenna: [RF24](https://nrf24.github.io/RF24). Installation via Arduino-IDE.

## Overview

`reciever_module/` - folder with code for reciver which will send data to the pc (arduino)
`sender_module/` - folder with code for sender, which transmit data to the reciever and save it on the micro sd card (arduino)
`serial_read/` - read serial input and save it
`monitor` - folder with code for monitor which will recieve data and print them into the gui application (pc)
`testing/` - other tools and applications

### Schema

#### Sender

- device (arduino uno, arduino nano or arduino micro...) which recieves data from *[10DOF IMU](#10-dof-imu)* and *[L76K GPS](#l76k-gps)*, then it sends them via *[nrf24l01+](#nrf24l01)* in [sender format](#arduino-sender-format) to [reciever](#reciever)

#### Reciever

- device (arduino uno r4 wifi, arduino uno or arduino nano...) which recieves data from [sender](#sender) via *[nrf24l01+](#nrf24l01)* and then send them through serial line to the [master device](#master) in the [sender format](#arduino-sender-format)

#### Master

- device (pc, notebook, laptop), where the data are processed, parsed, saved and displayed realtime in the application monitor

### Arduino Sender Format

- sender sends data via antenna to reciever in this format **$[code of message];[value]\***
- in future will be added some other values, like gps and so on

| Identifier | Message Code | Value                            | Verificator |
| ---------- | ------------ | -------------------------------- | ------------|
| $          | **1**;       | roll                             | *           |
| $          | **2**;       | pitch                            | *           |
| $          | **3**;       | yaw                              | *           |
| $          | **4**;       | temperature [degrees of Celsius] | *           |
| $          | **5**;       | pressure                         | *           |
| $          | **6**;       | altitude                         | *           |
| $          | **7**;       | gyroscope x                      | *           |
| $          | **8**;       | gyroscope y                      | *           |
| $          | **9**;       | gyroscope z                      | *           |
| $          | **10**;      | accelerometer x                  | *           |
| $          | **11**;      | accelerometer y                  | *           |
| $          | **12**;      | accelerometer z                  | *           |
| $          | **13**;      | magnitude x                      | *           |
| $          | **14**;      | magnitude y                      | *           |
| $          | **15**;      | magnitude z                      | *           |

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

> [!TIP]
> Fix Quality:
> 0 = Invalid;
> 1 = GPS fix;
> 2 = DGPS fix;
> [more about nmea sentences](https://aprs.gids.nl/nmea/)

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

## Issues / features

### Sender issues

- [x] data stops being transmitted from sender after some short period time
- [ ] create a version which will send data via **IOT 433MHz LoRa LPWAN SX1278**
- [ ] create a communication in both ways, `start`, `stop`, `system health check` commands
- [ ] detection of apogeum and recovery system launch

### Monitor app issues

- [ ] application crash after some period of time, if don't recive any input from serial ports
- [ ] gui is not updating until it recieves serial input
- [ ] parser should be improved
- [ ] sender code should be improved
- [x] gui window
- [ ] error messages as windows not terminal
- [ ] improve readability of code
- [ ] serial monitor setup port and baudrate
- [ ] create a gui way of sending commands

## Sources

Datasheets, documentation and sources

- [Waveshare 10 DOF IMU Sensor (D) - Arduino guide](https://www.waveshare.com/wiki/Template:10_DOF_IMU_Sensor_D_Arduino_Guide)
- [Waveshare 10 DOF IMU Sensor (C) - Demo code](https://files.waveshare.com/upload/c/c1/10_DOF_IMU_Sensor_D_Code.7z)
- [Waveshare 10 DOF IMU Sensor (C) - Documentation](https://www.waveshare.com/wiki/10_DOF_IMU_Sensor_(C))
- [Waveshare L76K GPS Module - Arduino guide](https://www.waveshare.com/wiki/L76K_GPS_Module#Working_With_Arduino)
- [Waveshare L76K GPS Module - Demo code](https://files.waveshare.com/upload/9/94/L76K_GPS_Module_Arduino.zip)
- [NRF24L01+ PA/LNA - Arduino guide](https://lastminuteengineers.com/nrf24l01-arduino-wireless-communication/)
- [NRF24L01+ Guide - Guide](https://navody.dratek.cz/navody-k-produktum/arduino-wifi-modul-nrf24l01.html)
- [NRF24L01+ PA/LNA - Datasheet](doc/nrf24lo1-datasheet.pdf)
- [NRF24L01+ PA/LNA - Demo code](https://img.gme.cz/files/eshop_data/eshop_data/10/775-034/prg.775-034.1.zip)
- [Pixel2 (go gui library) - Github repository](https://github.com/gopxl/pixel)
- [LoraWAN Library](https://github.com/mcci-catena/arduino-lmic)
- All other reference materials are in the *doc/* directory
