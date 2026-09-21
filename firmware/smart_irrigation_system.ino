int moisturevalue ;
int moisturetopump ;
int currentvalue;
int waterPvalue;
int waterNvalue;

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

// RemoteXY connection settings
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 135 bytes
  { 255,2,0,23,0,128,0,19,0,0,0,0,31,1,106,200,1,1,7,0,
  67,27,25,54,12,69,16,26,11,4,22,43,66,11,128,8,26,129,28,17,
  31,7,0,24,70,108,111,119,32,114,97,116,101,58,0,129,30,93,43,7,
  0,24,83,111,105,108,32,109,111,105,115,116,117,114,101,58,0,67,28,102,
  53,12,69,16,26,11,74,27,144,56,9,21,2,31,37,64,76,111,119,32,
  119,97,116,101,114,32,108,101,118,101,108,0,37,64,72,105,103,104,32,99,
  117,114,114,101,110,116,0,4,24,122,64,10,128,8,26 };

// this structure defines all the variables and events of your control interface
struct {

   // input variables
 int8_t slider_01; // from 0 to 100
 int8_t slider_02; // from 0 to 100

   // output variables
 char value_01[11]; // string UTF8 end zero
 char value_02[11]; // string UTF8 end zero
 uint8_t strings_01; // from 0 to 2

   // other variable
 uint8_t connect_flag;   // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

void setup() {
  RemoteXY_Init ();
  Serial.begin(115200);
  pinMode(4,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  RemoteXY.slider_01= 100;
  RemoteXY.slider_02= 50;
  RemoteXY.strings_01=0;
}
void loop() {
  RemoteXY_Handler ();
  digitalWrite(4,LOW);
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  int moisturevalueR=analogRead(A0);
  moisturevalue= 1023-moisturevalueR;
  currentvalue=analogRead(A4);
  if (moisturevalue < RemoteXY.slider_02*10.23){
    if(RemoteXY.slider_02==50){
      moisturetopump = map(moisturevalueR, 0, 1023, 0, 255);
    } else {
      moisturetopump=map(RemoteXY.slider_01,0,100,0,255);
    }
  } else{
    moisturetopump=0;
  }
  if (currentvalue>700){
     RemoteXY.strings_01=2;
     moisturetopump=0;
   digitalWrite(4,HIGH);
  }
  waterNvalue=analogRead(A1);
  if (waterNvalue<100){
     RemoteXY.strings_01=1;
     moisturetopump=0;
     digitalWrite(4,HIGH);
    }
    waterPvalue=waterNvalue;
    RemoteXY_delay(1000);
    waterNvalue=analogRead(A1);
    moisturevalue=map(moisturevalue,0,1023,0,100);
    dtostrf(moisturevalue, 0, 1, RemoteXY.value_02);
    dtostrf((waterPvalue-waterNvalue)*0.52,0,1,RemoteXY.value_01);
    analogWrite(10,moisturetopump);
}
