#include <stdio.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <HMC5883L.h>
#include <MS5611.h>
#include <SD.h>

const int chipSelect = 4;
char file_name[30];

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

void setup(){
  //Init Pins
  Serial.begin(38400);
  Serial.println(F("Initialize Pins"));
  delay(100);

  //Init Serial Port
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
  pinMode(SS, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present"));
    // 失敗、何もしない
    while(1);
  }
  // 日付と時刻を返す関数を登録
  SdFile::dateTimeCallback( &dateTime );
  int file_no = 0;
  while(1){
    snprintf(file_name, sizeof(file_name), "data_%d.dat", file_no);
    if(SD.exists(file_name)){
      file_no++;
    }
    else{
      break;
    }
  }

  // Date and time setup
  SdFile::dateTimeCallback( &dateTime );
  Serial.println(F("Get Time Stamp"));
}

void loop(){
  // ファイルを開く
  File dataFile = SD.open(file_name, FILE_WRITE);

  // もしファイルが開けたら値を書き込む
  if (dataFile) {
    //Get Motion Data
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    //Get Magnetometer
    mag.getHeading(&mx, &my, &mz);

    dataFile.print(ax);
    dataFile.print(ay);
    dataFile.print(az);
    dataFile.print(gx);
    dataFile.print(gy);
    dataFile.print(gz);
    dataFile.print(mx);
    dataFile.print(my);
    dataFile.print(mz);
    dataFile.println("");

    Serial.print(ax);
    Serial.print(ay);
    Serial.print(az);
    Serial.print(gx);
    Serial.print(gy);
    Serial.print(gz);
    Serial.print(mx);
    Serial.print(my);
    Serial.print(mz);
    Serial.println("");
    dataFile.close();
  }
  // ファイルが開けなかったらエラーを出力
  else {
    Serial.println(F("error opening datalog.txt"));
  } 

  // 一秒待つ
  delay(10);
}

void logging(){

}

void dateTime(uint16_t* date, uint16_t* time){
  uint16_t year = 2013;
  uint8_t month = 6, day = 3, hour = 21, minute = 23, second = 0;

  *date = FAT_DATE(year, month, day);
  *time = FAT_TIME(hour, minute, second);
}



