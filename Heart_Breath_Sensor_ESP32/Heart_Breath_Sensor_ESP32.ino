#include "Arduino.h"
#include <Wire.h>
// #include <LiquidCrystal_I2C.h>
#include <Wire.h>
// #include <Adafruit_MLX90614.h>
#include <WiFi.h>
#include <WiFiClient.h>
// #include <BlynkSimpleEsp32.h>
// #define BLYNK_PRINT Serial
// #define BLYNK_TEMPLATE_ID "TMPL3gy4Fq2LC"
// #define BLYNK_TEMPLATE_NAME "Project"
// #define BLYNK_AUTH_TOKEN "
// char auth[] = "cxwnZjdeqqhWcGuObkEZ5OVt1WC_227g";
// char ssid[] = "wifi";
// char pass[] = "123456788";
// Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define RXD2 16
#define TXD2 17
#include <60ghzbreathheart.h>
// LiquidCrystal_I2C lcd(0x27, 16, 2);
BreathHeart_60GHz radar = BreathHeart_60GHz(&Serial2);
unsigned long delay_Intial = 0;
const long delay_interval = 1000;
unsigned long measure_Intial = 0;
const long measure_interval = 20000;
int Human_pesence_pin = 27;
int Human_presence;
int Heart_rate_measure_button_pin = 26;
int Heart_Button = 0;
int IR_Sensor_pin = 33;
int IR_Status;
int relay = 32;
int Buzzer = 25;
void _delay() {
  delay_Intial = millis();
  while ((millis() - delay_Intial) <= delay_interval) {
  }
}



void Measure_Breath_Heart_rate() {
  radar.Breath_Heart();  //Breath and heartbeat information output
  if (radar.sensor_report != 0x00) {
    switch (radar.sensor_report) {
      case HEARTRATEVAL:
        Serial.print("Sensor monitored the current heart rate value is: ");
        Serial.print(radar.heart_rate, DEC);
        Serial.println();
        // lcd.clear();
        // lcd.setCursor(0,0);
        // lcd.print("Heart Rate: ");
        // lcd.print(radar.heart_rate);
        // Blynk.virtualWrite(V0,radar.heart_rate);
        break;
      case BREATHVAL:
        Serial.print("Sensor monitored the current breath rate value is: ");
        Serial.print(radar.breath_rate, DEC);
        Serial.println();
        // lcd.setCursor(0,1);
        // lcd.print("Breath_rate: ");
        // lcd.print(radar.breath_rate, DEC);
        // Blynk.virtualWrite(V1,radar.breath_rate);
        break;
    }
  }
}

// void human_presence()
// {
//   radar.HumanExis_Func();           //Human existence information output
//   if(radar.sensor_report != 0x00){
//     switch(radar.sensor_report){
//       case BODYVAL:
//         Serial.print("The parameters of human body signs are: ");
//         Serial.println(radar.bodysign_val, DEC);
//         Serial.println("----------------------------");
//         if(radar.bodysign_val >=15)
//         {
//           Blynk.virtualWrite(V3,20);
//           measure_Intial = millis();
//           delay(1000);
//           Blynk.virtualWrite(V3,0);
//           while((millis() - measure_Intial) <= measure_interval)
//           {
//             digitalWrite(relay,HIGH);
//             lcd.clear();
//             lcd.setCursor(0,0);
//             lcd.print("Presence Found");
//             lcd.setCursor(0,1);
//             lcd.print("Lights ON");
//             _delay();
//           }

//           digitalWrite(relay,LOW);
//           lcd.setCursor(0,1);
//           lcd.print("Lights OFF");
//           _delay();
//         }
//         break;
//     }
//   }
// }

void HR_BR() {
  measure_Intial = millis();
  while ((millis() - measure_Intial) <= measure_interval) {
    Measure_Breath_Heart_rate();
  }
}

void Measure_IR_Temperature() {
  // Serial.print("*C\tObject = ");
  // Serial.print(mlx.readObjectTempC());
  // Serial.println("*C");
  // lcd.clear();
  // lcd.setCursor(0,1);
  // lcd.print("Body Temp: ");
  // lcd.print(mlx.readObjectTempC());
  // Blynk.virtualWrite(V2,mlx.readObjectTempC());
  _delay();
}

void setup() {
  Serial.begin(115200);                           //RX0,TX0 OF ESP32
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  //RX2,TX2 OF ESP32
  pinMode(Heart_rate_measure_button_pin, INPUT);
  pinMode(IR_Sensor_pin, INPUT);
  pinMode(Human_pesence_pin, INPUT);
  // pinMode(relay,OUTPUT);
  // pinMode(Buzzer,OUTPUT);
  // Wire.begin();
  // lcd.begin();
  // lcd.backlight();
  // mlx.begin();
  // radar.ModeSelect_fuc(1);
  // Blynk.begin(auth, ssid, pass);
}

void loop() {
  Heart_Button = digitalRead(Heart_rate_measure_button_pin);
  // IR_Status = digitalRead(IR_Sensor_pin);
  Human_presence = digitalRead(Human_pesence_pin);

  Serial.println("Heart_rate_monitoring_system_acitve");
  HR_BR();
  // lcd.clear();
  // digitalWrite(relay,LOW);
  // digitalWrite(Buzzer,LOW);
  // Blynk.virtualWrite(V0,0);
  // Blynk.virtualWrite(V1,0);
  // Blynk.virtualWrite(V2,0);
  // Blynk.virtualWrite(V3,0);
  // switch(IR_Status)
  // {
  //   case 1:
  //     Measure_IR_Temperature();
  //     break;
  //   case 0:
  //     break;
  //     lcd.clear();
  // }

  // switch(Heart_Button)
  // {
  //   case 1:
  //     digitalWrite(Buzzer,HIGH);
  //     _delay();
  //     digitalWrite(Buzzer,LOW);
  //     _delay();

  //     break;
  // }

  // switch(Human_presence)
  // {
  //   case 1:
  //     Serial.println("Human_presence_detection_active");
  //     human_presence();
  //     break;
  //   case 0:
  //     digitalWrite(relay,LOW);
  //     break;
  // }
  // Blynk.run();
}