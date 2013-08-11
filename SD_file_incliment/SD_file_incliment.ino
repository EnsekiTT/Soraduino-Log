#include <stdio.h>
#include <SD.h>

const int chipSelect = 4;
char file_name[30];

void setup()
{
  Serial.begin(38400);
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
    }else{
      break;
    }
  }
}

void loop()
{
  // ファイルを開く
  File dataFile = SD.open(file_name, FILE_WRITE);

  // もしファイルが開けたら値を書き込む
  if (dataFile) {
    int value = analogRead(0);
    dataFile.println(value);
    dataFile.close();
    // シリアルポートにも出力
    Serial.println(value);
  }
  // ファイルが開けなかったらエラーを出力
  else {
    Serial.println(F("error opening datalog.txt"));
  } 

  // 一秒待つ
  delay(10);
}

void dateTime(uint16_t* date, uint16_t* time)
{
  uint16_t year = 2013;
  uint8_t month = 2, day = 3, hour = 9, minute = 0, second = 0;

  // GPSやRTCから日付と時間を取得
  // FAT_DATEマクロでフィールドを埋めて日付を返す
  *date = FAT_DATE(year, month, day);

  // FAT_TIMEマクロでフィールドを埋めて時間を返す
  *time = FAT_TIME(hour, minute, second);
}
