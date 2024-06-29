#include <SPI.h>
#include <SD.h>

#include "Arduino_BMI270_BMM150.h"
#include <nRF24L01.h>
#include <RF24.h>

// Define pin numbers for modules
const int NRF_CE_PIN = 9;
const int NRF_CS_PIN = 8;
const byte address[6] = "00001";

const int BUZZER_PIN = 7;
const int READY_STAGE_PIN = 5;
const int LAUNCH_STAGE_PIN = 6;

// Create objects for modules
RF24 radio(NRF_CE_PIN, NRF_CS_PIN);
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
  
  while (!Serial);
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
      arm_stage();
      break;
    case ASCENT:
      //ascent_stage();
      break;
    case DESCENT:
      //descent_stage();
      break;
    case LANDED:
      //landed_stage();
      break;
  }
}

bool isButtonPressed(int buttonPin) { return digitalRead(buttonPin) == HIGH;}

void ready_stage() {
  Serial.println("# READY stage");
  int counter = 0;
  const int buttonPin = 2; 

  while (true) {
    if (isButtonPressed(buttonPin)) {
    Serial.println("Button is pressed!"); 
    break;
  } else {
    Serial.println("Button is not pressed."); 
  }
  delay(100);
  }

  current_stage = ARM;
}

void arm_stage() {
  // System check
  // Block parachute ejection
  // Wait for launch pin removed
  // Start sending data
  Serial.println("# ARM stage");

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
  
  // Accelerometer Gyroscope
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  
  // Accelerometer Gyroscope
  int counter = 0;
  float x, y, z;

  while (true) {
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);

      Serial.println(x);
      if ( x > 1) {
        Serial.println("# Launch Detect");
        counter++;
      }
    }

    if (counter > 10) {
      Serial.println("# Launching");
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
