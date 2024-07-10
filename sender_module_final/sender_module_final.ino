#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#include "Arduino_BMI270_BMM150.h"
#include <nRF24L01.h>
#include <RF24.h>

// Define pin numbers for modules
const byte address[6] = "00001";

const int BUZZER_PIN = 2;
const int SERVO_A_PIN = 3;
const int CHIP_SELECT = 4;
const int SERVO_B_PIN = 5;
const int NRF_CS_PIN = 8;
const int NRF_CE_PIN = 9;

// Create objects for modules
RF24 radio(NRF_CE_PIN, NRF_CS_PIN);
File dataFile;
Servo A;
Servo B;

// Flight stages
enum FlightStage
{
  READY,
  ARM,
  ASCENT,
  DESCENT,
  LANDED
};

FlightStage current_stage = READY;

void setup()
{
  // Initialize Serial
  Serial.begin(115200);

  while (!Serial)
    ;
  Serial.println("# Welcome to CobraV2 operating system for rocket");
  delay(1000);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  A.attach(5); // attaches the servo on pin 9 to the servo object
  B.attach(6);
  // Start with preflight stage
  current_stage = READY;

  delay(1000);
}

void loop()
{
  switch (current_stage)
  {
  case READY:
    ready_stage();
    break;
  case ARM:
    arm_stage();
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

void beep()
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}

void ready_stage()
{
  Serial.println("# READY stage");
  analogWrite(LED_BUILTIN, HIGH);
  beep();

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  // Radio
  // radio.begin();
  // radio.openWritingPipe(address);
  // radio.stopListening(); // Set module as transmitter
  // const char msg[] = "# Radio connection activated";
  // radio.write(&msg, sizeof(msg));
  // Serial.println("# Success nRF24L01 init");

  // SD card
  //if (!SD.begin(CHIP_SELECT))
  //{
  //  Serial.println("# Fail SD module init");
  //}
  //else
  //{
  //  Serial.println("# Success SD module init");
  //}
//
  //if (SD.exists("data.txt"))
  //{
  //  Serial.println("# File exists");
  //}
  //else
  //{
  //  Serial.println("# File does not exist");
  //  dataFile = SD.open("data.txt", FILE_WRITE);
  //}
//
  //if (dataFile)
  //{
  //  Serial.println("# File opened");
  //  dataFile.println("# CobraV2 flight data");
  //}
  //else
  //{
  //  Serial.println("# Error opening file");
  //}

  Serial.print("# Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");

  //dataFile.print("# Accelerometer sample rate = ");
  //dataFile.println(IMU.accelerationSampleRate());
  //dataFile.close();

  current_stage = ARM;
}

void arm_stage()
{
  // System check
  // Block parachute ejection
  // Wait for launch pin removed
  // Start sending data
  Serial.println("# ARM stage");
  beep();
  beep();

  // Accelerometer Gyroscope
  int counter = 0;
  float x, y, z;

  bool toggle = false;
  //dataFile = SD.open("data.txt", FILE_WRITE);
  //dataFile.println("# ARM stage");
  while (true)
  {
    if (IMU.accelerationAvailable())
    {
      IMU.readAcceleration(x, y, z);
      Serial.println(x);

      //dataFile.print(x);
      //dataFile.print(" ");
      //dataFile.print(y);
      //dataFile.print(" ");
      //dataFile.println(z);

      if (x > 1)
      {
        Serial.println("# Launch Detect");
        counter++;
      }
      else
      {
        counter = 0;
      }
    }

    if (counter < 15)
    {
      Serial.println("# Launching");
      //dataFile.println("# Launching");
      break;
    }

    if (toggle)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
    toggle = !toggle;
  }

  current_stage = ASCENT;
  //dataFile.close();
}

void ascent_stage()
{
  Serial.println("# ASCENT Stage");
  unsigned long StartTime = millis();
  int FailOrientationCounter = 0;
  //File dataFile = SD.open("data.txt", FILE_WRITE);
  //dataFile.println("# ASCENT stage");
  while (true)
  {
    unsigned long CurrentTime = millis();
    unsigned long ElapsedTime = CurrentTime - StartTime;
    Serial.println(ElapsedTime);

    if (IMU.gyroscopeAvailable())
    {
      float x, y, z;
      IMU.readGyroscope(x, y, z);
    }

    if ((ElapsedTime > 9000))
    {
      dataFile.println("# Apogee detected by time");
      break;
    }
  }

  current_stage = DESCENT;
  //dataFile.close();
}

void descent_stage()
{
  Serial.println("# DESCENT stage");
  //File dataFile = SD.open("data.txt", FILE_WRITE);
  //dataFile.println("# DESCENT stage");
  for (int pos = 0; pos <= 150; pos += 1)
  {
    // in steps of 1 degree
    A.write(pos);
    delay(1);
    Serial.println(pos);
  }

  for (int pos = 90; pos >= 0; pos -= 1)
  {
    B.write(pos);
    delay(1);
    Serial.println(pos);
  }
  Serial.println("# Parachute deployed");

  unsigned long StartTime = millis();
  while (true)
  {
    unsigned long CurrentTime = millis();
    unsigned long ElapsedTime = CurrentTime - StartTime;

    if (ElapsedTime > 300000)
    {
      //dataFile.println("# Landing detected");
      break;
    }
  }
  current_stage = LANDED;
  //dataFile.close();
}

void landed_stage()
{
  while (true)
  {
    Serial.println("# LANDED stage");
    beep();
    delay(200);
  }
}
