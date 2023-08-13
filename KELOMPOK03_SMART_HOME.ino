
#define BLYNK_PRINT Serial


#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"
#define NUM_LEDS 53
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define PIN1 D2

CRGB leds[NUM_LEDS];
int data=255;
int r,g,b;
int redLightAmount;
int greenLightAmount;
int blueLightAmount;
int redPinValue;
int greenPinValue;
int bluePinValue;

BlynkTimer timer;

// Token autentifikasi dari app blynk
// Pergi ke Project Settings (nut icon).
char auth[] = "XKWpNUM7H887G57yF1SjzonFTI9JPMON";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SmartHome AP1 2.4G";
char pass[] = "Smarthome";

int minTempValue;
int autoFanValue;
String buttonOffMsg = "Automatic Fan System is disabled.";
String fanOnMsg = "Fan Status: ON";
String fanOffMsg = "Fan Status: OFF";

BLYNK_WRITE(V0)
{
  minTempValue = param.asInt();
} 

BLYNK_WRITE(V1)
{
  autoFanValue = param.asInt();
}

BLYNK_WRITE(V2)
{
  buttonOffMsg = param.asString();
  fanOnMsg = param.asString();
  fanOffMsg = param.asString();
}


DHT dht_3(D9,11);
void Event1()
{Blynk.virtualWrite(V6, dht_3.readTemperature());}
void Event2()
{Blynk.virtualWrite(V5, dht_3.readHumidity());}

void setup()
{



  digitalWrite(D3,HIGH);

  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D8,OUTPUT);

void loop()
{  
  Blynk.run();
  timer.run();

  //Serial.println(autoFanValue);

if (autoFanValue > 0) {
  if (dht_3.readTemperature() > minTempValue) {
    digitalWrite(D1,HIGH);
    Blynk.virtualWrite(V2,fanOnMsg);
    }

  if (dht_3.readTemperature() < minTempValue) {
    digitalWrite(D1,LOW);
    Blynk.virtualWrite(V2,fanOffMsg);
  }
}
else {
  digitalWrite(D1,LOW);
  Blynk.virtualWrite(V2,buttonOffMsg);
}

}
