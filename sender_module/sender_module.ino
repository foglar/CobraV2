#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Waveshare_10Dof-D.h"
#include "string.h"


bool gbSenserConnectState = false;
//create an RF24 objetct
RF24 radio(9, 8);                 // CE, CSN
const byte address[6] = "00001";  //address through which two modules communicate

void setup() {
  // init radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();  //Set module as transmitter

  // init motion sensor
  bool bRet;
  IMU_EN_SENSOR_TYPE enMotionSensorType, enPressureType;
  Serial.begin(115200);

  imuInit(&enMotionSensorType, &enPressureType);
  if(IMU_EN_SENSOR_TYPE_ICM20948 == enMotionSensorType)
  {
    Serial.println("# Motion sersor is ICM-20948");
  }
  else
  {
    Serial.println("# Motion sersor NULL");
  }
  if(IMU_EN_SENSOR_TYPE_BMP280 == enPressureType)
  {
    Serial.println("# Pressure sersor is BMP280");
  }
  else
  {
    Serial.println("# Pressure sersor NULL");
  }
  delay(1000);
}

void loop() {
  IMU_ST_ANGLES_DATA stAngles;
  IMU_ST_SENSOR_DATA stGyroRawData;
  IMU_ST_SENSOR_DATA stAccelRawData;
  IMU_ST_SENSOR_DATA stMagnRawData;
  int32_t s32PressureVal = 0, s32TemperatureVal = 0, s32AltitudeVal = 0;
  
  imuDataGet( &stAngles, &stGyroRawData, &stAccelRawData, &stMagnRawData);
  pressSensorDataGet(&s32TemperatureVal, &s32PressureVal, &s32AltitudeVal);
  char result[8];
  float temperature = (s32TemperatureVal / 100);
  dtostrf(temperature, 6, 2, result);
  //Send message to receiver
  radio.write(&result, sizeof(result));
  //delay(1000);
}