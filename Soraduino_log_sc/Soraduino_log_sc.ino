#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <HMC5883L.h>
#include <MS5611.h>
#include <SD.h>

#define FILENAME "datalog.txt"

//MPU6050
MPU6050 mpu;
int16_t ax,ay,az;
int16_t gx,gy,gz;

//HMC5883L
HMC5883L mag;
int16_t mx, my, mz;

//MS5611
MS5611 ms;
int32_t d1, d2;

//SD card
const int chipSelect = 4;
File dataFile;

void setup(){
  //Init Pins
  pinMode(SS, OUTPUT);
  Serial.println(F("Initialize Pins"));
  delay(100);
  
  //Init Serial Port
  Serial.begin(38400);
  Serial.println(F("Initialize Start"));
  
  //Init Wire
  Wire.begin();
  Serial.println(F("Initialize wire"));
  
  //Init MPU6050
  mpu.initialize();
  Serial.println(F("Initialize MPU6050"));
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  delay(100);
  
  //Init HMC5883L
  mag.initialize();
  Serial.println(F("Initialize HMC5883L"));
  Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
  
  //Init MS5611
  ms.initialize();
  Serial.println(F("Initialize MS5611"));
  //Serial.println(ms.testConnection() ? "MS5611 connection successful" : "MS5611 connection failed");
  
  //Init SD Library
  if(!SD.begin(chipSelect)){
    Serial.println(F("Card faild"));
    while(1); 
  }
  while(1){
    dataFile = SD.open(FILENAME, FILE_WRITE);
    if(dataFile) break;
    delay(100);
  }
  Serial.println(F("Initialize SD Card"));
  
  // Date and time setup
  SdFile::dateTimeCallback( &dateTime );
  Serial.println(F("Get Time Stamp"));
}

void loop(){
  //Get Motion Data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  //Get Magnetometer
  mag.getHeading(&mx, &my, &mz);
  
  dataFile.println(ax);
  Serial.print(mx);
  Serial.print(",");
  Serial.println(ax);
  
  delay(100);
}

void close(){
  dataFile.close(); 
}

void logging(){

}

void dateTime(uint16_t* date, uint16_t* time){
  uint16_t year = 2013;
  uint8_t month = 6, day = 3, hour = 21, minute = 23, second = 0;

  *date = FAT_DATE(year, month, day);
  *time = FAT_TIME(hour, minute, second);
}


