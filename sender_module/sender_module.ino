#include <SPI.h>
#include <SD.h>

#include <nRF24L01.h>
#include <RF24.h>
//#include <Adafruit_INA219.h>

#include "DEV_Config.h"
//#include "L76X.h"
//#include "Waveshare_10Dof-D.h"

// Define pin numbers for modules
const int NRF_CE_PIN = 9;
const int NRF_CS_PIN = 8;
const byte address[6] = "00001";

//const int GPS_TX_PIN = 0;
//const int GPS_RX_PIN = 1;

//const String INA219_SDA_PIN = "A4";
//const String INA219_SCL_PIN = "A5";

const int BUZZER_PIN = 7;
const int READY_STAGE_PIN = 5;
const int LAUNCH_STAGE_PIN = 6;

// Create objects for modules
RF24 radio(NRF_CE_PIN, NRF_CS_PIN);
//GNRMC GPS1;
//Adafruit_INA219 ina219;
File dataFile;

// Flight stages
enum FlightStage {
  READY,
  ARM,
  ASCENT,
  DESCENT,
  LANDED
};

FlightStage current_stage = READY;

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  Serial.println("# Welcome to CobraV2 operating system for rocket");
  delay(1000);

  // Buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(READY_STAGE_PIN, INPUT); 
  
  // Start with preflight stage
  current_stage = READY;

  delay(1000);
}

void loop() {
  switch (current_stage) {
    case READY:
      ready_stage();
      break;
    case ARM:
      arm_stag();
      break;
    case ASCENT:
      ascent_stage();
      break;
    case DESCENT:
      descent_stage();
      break;
    case LANDED:
      landed_stage();
      break;
  }
}

void ready_stage() {
  Serial.println("# READY stage");
  int counter = 0;

  while (true) {
    int state = digitalRead(READY_STAGE_PIN);
    if (state == HIGH) {
      counter = 0;
    } else {
      counter += 1;
      delay(300);
    }

    if (counter == 10) {
      Serial.println("# Pin disconnected");
      break;
    }
  }

  current_stage = READY;
}

void arm_stage() {
  // System check
  // Block parachute ejection
  // Wait for launch pin removed
  // Start sending data
  Serial.println("# READY stage");
  // Radio
  if (!radio.begin()) {
    Serial.println("# Fail nRF24L01 init");
  } else {
    radio.openWritingPipe(address);
    radio.stopListening();  // Set module as transmitter
    const char msg[] = "# Radio connection activated";
    radio.write(&msg, sizeof(msg));
    Serial.println("# Success nRF24L01 init");
  }
  
  // GPS
  //DEV_Set_Baudrate(9600);
  //DEV_Delay_ms(500);
  
  // IMU
  //IMU_EN_SENSOR_TYPE enMotionSensorType, enPressureType;
  //imuInit(&enMotionSensorType, &enPressureType);
  //if (IMU_EN_SENSOR_TYPE_ICM20948 == enMotionSensorType) {
  //  Serial.println("# Success ICM-20948 init");
  //  Serial.println(IMU_EN_SENSOR_TYPE_ICM20948);
  //} else {
  //  Serial.println("# Fail ICM-20948 init");
  //}
  //if (IMU_EN_SENSOR_TYPE_BMP280 == enPressureType) {
  //  Serial.println("# Success BMP280 init");
  //} else {
  //  Serial.println("# Fail BMP280 init");
  //  Serial.println(enPressureType);
  //}
  
  // INA219
  // if (!ina219.begin()) {
  //   Serial.println("# Fail INA219 sensor init");
  // } else {
  //   Serial.println("# Success INA219 sensor init");
  // }
  
  // SD card
  if (!SD.begin(4)) {
    Serial.println("# Fail SD module init");
  } else {
    Serial.println("# Success SD module init");
  }

  if (SD.exists("data.txt")) {
    Serial.println("# File exists");
  } else {
    Serial.println("# File does not exist");
  }

  dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    Serial.println("# File opened");
    dataFile.println("# CobraV2 flight data");
  } else {
    Serial.println("# Error opening file");
  }

  // Buzzer
  tone(BUZZER_PIN, 1000);
  delay(1000);
  noTone(BUZZER_PIN);

  // Check battery level
  // float batteryLevel = getBatteryLevel();
  // Serial.print("# Battery level: ");
  // Serial.print(batteryLevel);
  // Serial.println("%");

  // Check for launch pin
  int counter = 0;
  while (true) {
    int state = digitalRead(LAUNCH_STAGE_PIN);
    if (state == HIGH) {
      counter = 0;
    } else {
      counter += 1;
      delay(300);
    }

    if (counter == 10) {
      Serial.println("# Pin disconnected");
      break;
    }
  }
  current_stage = ASCENT;
}

void ascent_stage() {
  // Launch
  // Start sending and saving data on SD card
  // Check for apogee
  // Eject parachute
}

void descent_stage() {
  // Detect apogee with accelerometer
  // Eject parachute
  // GPS and height check
}

void landed_stage() {
  // Check for zero velocity
  // Shut down unneeded systems
  // Buzz on
  // Send GPS location
  // Battery check
  // Turn off gyro and accelerometer
}

//float getBatteryLevel() {
//  // Read bus voltage from INA219
//  float busVoltage = ina219.getBusVoltage_V();
//
//  // Assuming fully charged battery voltage is 4.2V and empty is 3.0V
//  float maxVoltage = 4.2;
//  float minVoltage = 3.0;
//
//  // Map the bus voltage to a battery level percentage
//  float batteryLevel = map(busVoltage, minVoltage, maxVoltage, 0, 100);
//
//  // Constrain the battery level to be within 0 and 100
//  batteryLevel = constrain(batteryLevel, 0, 100);
//
//  return batteryLevel;
//}

