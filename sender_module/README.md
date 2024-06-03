# Sender module

## Stages of flight

1. Preflight - Remove before flight button or smthing like that
    - no communication, blocked parachute ejection, all systems down
2. Ready - Signal from groundstation
    - sys check (modules calibration, battery check, atd...), blocked parachute ejection, wait for launch pin removed, only send data, do not save them
3. Launch - Removed pin from the rocket
    - launch, all systems sending and saving data on sd card
4. Apogee - Detected that rocket is in apogee with accelerometer
    - parachute ejection, all systems working, gps check and height check
5. Return - Rocket has no velocity
    - all not needed systems shutdown/sleep, buzz on, gps sending location, battery check, turn off gyro and accelerometer

## Modules

- nRF24L01 - Communication with groundstation
- L76K GPS - GPS sensor for gps data
- 10 DOF IMU - Gyroscope/Accelerometer, Temperature, atd.
- ?INA-219 - Battery data
- ?SD card reader - Write all recieved data to the SD card
- ?Buzzer - To find our rocket after launch

## Tasks 

- Ejection mechanism
- PCB for our computer
- force parachute ejection
- if cable connected return back to ready stage
- send signal of listening
- wait for recieve
