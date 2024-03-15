#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Waveshare_10Dof-D.h"

//----------------------- CONSTANTS -----------------------//
bool gbSenserConnectState = false;
RF24 radio(9, 8);                 // CE, CSN
const byte address[6] = "00001";  // address through which two modules communicate

//----------------------- SETUP -----------------------//
void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();  // Set module as transmitter

  IMU_EN_SENSOR_TYPE enMotionSensorType, enPressureType;
  Serial.begin(115200);
  imuInit(&enMotionSensorType, &enPressureType);
  if (IMU_EN_SENSOR_TYPE_ICM20948 == enMotionSensorType) {
    Serial.println("# Motion sensor is ICM-20948");
  } else {
    Serial.println("# Motion sensor NULL");
  }
  if (IMU_EN_SENSOR_TYPE_BMP280 == enPressureType) {
    Serial.println("# Pressure sensor is BMP280");
  } else {
    Serial.println("# Pressure sensor NULL");
  }
  delay(1000);
}

//----------------------- LOOP -----------------------//
void loop() {
  IMU_ST_ANGLES_DATA stAngles;
  IMU_ST_SENSOR_DATA stGyroRawData;
  IMU_ST_SENSOR_DATA stAccelRawData;
  IMU_ST_SENSOR_DATA stMagnRawData;
  int32_t s32PressureVal = 0, s32TemperatureVal = 0, s32AltitudeVal = 0;
  
  imuDataGet(&stAngles, &stGyroRawData, &stAccelRawData, &stMagnRawData);
  pressSensorDataGet(&s32TemperatureVal, &s32PressureVal, &s32AltitudeVal);
  
  float temperature = s32TemperatureVal / 100.0;
  float pressure = s32PressureVal / 100.0;
  float altitude = s32AltitudeVal / 100.0;

  float angles[] = {stAngles.fRoll, stAngles.fPitch, stAngles.fYaw};
  float gyro[] = {stGyroRawData.s16X, stGyroRawData.s16Y, stGyroRawData.s16Z};
  float accel[] = {stAccelRawData.s16X, stAccelRawData.s16Y, stAccelRawData.s16Z};
  float magn[] = {stMagnRawData.s16X, stMagnRawData.s16Y, stMagnRawData.s16Z};
  
  char msg[64];
  for (int i = 0; i < 3; i++) {
    char float_str[8];
    dtostrf(angles[i], 6, 2, float_str);
    String str = String("$") + String(i + 1) + ";" + String(float_str) + "*";
    str.toCharArray(msg, sizeof(msg));
    radio.write(&msg, sizeof(msg));
  }

  float sensor_data[][3] = {
    {temperature, pressure, altitude},
    {gyro[0], gyro[1], gyro[2]},
    {accel[0], accel[1], accel[2]},
    {magn[0], magn[1], magn[2]}
  };

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      char float_str[8];
      dtostrf(sensor_data[i][j], 6, 2, float_str);
      String str = String("$") + String(i + 4) + ";" + String(float_str) + "*";
      str.toCharArray(msg, sizeof(msg));
      radio.write(&msg, sizeof(msg));
    }
  }
}
