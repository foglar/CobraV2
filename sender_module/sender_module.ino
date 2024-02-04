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
  
  char temp_str[8], pressure_str[8], altitude_str[8];
  char roll_str[8], pitch_str[8], yaw_str[8];

  float temperature = (s32TemperatureVal / 100);
  float pressure = (s32PressureVal / 100);
  float altitude = (s32AltitudeVal / 100);

  float roll = stAngles.fRoll;
  float pitch = stAngles.fPitch;
  float yaw = stAngles.fYaw;

  dtostrf(temperature, 6, 2, temp_str);
  dtostrf(pressure, 6, 2, pressure_str);
  dtostrf(altitude, 6, 2, altitude_str);

  dtostrf(roll, 6, 2, roll_str);
  dtostrf(pitch, 6, 2, pitch_str);
  dtostrf(yaw, 6, 2, yaw_str);

  //Serial.println(roll, pitch, yaw);
  //Serial.println(temperature, pressure, altitude);
  
  char msg[64];

  String ctemp = ("$1;"+String(temp_str)+"*");
  String cpressure = ("$2;"+String(pressure_str)+"*");
  String caltitude = ("$3;"+String(altitude_str)+"*");
  String croll = ("$4;"+String(roll_str)+"*");
  String cpitch = ("$5;"+String(pitch_str)+"*");
  String cyaw = ("$6;"+String(yaw_str) + "*");

  ctemp.toCharArray(msg, sizeof(msg));
  radio.write(&msg, sizeof(msg));
  cpressure.toCharArray(msg, sizeof(msg));
  radio.write(&msg, sizeof(msg));
  
  delay(200);
}
