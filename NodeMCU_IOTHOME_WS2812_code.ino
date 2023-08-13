#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
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

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "XKWpNUM7H887G57yF1SjzonFTI9JPMON";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "POCO";
char pass[] = "12345678";

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

BLYNK_WRITE(V8)
//Adjusting slider for red colour on LED strip
{
  redPinValue = param.asInt();
  redLightAmount = redPinValue;

  if (redPinValue = 1) {
    white_led();
  }
}

BLYNK_WRITE(V9)
//Adjusting slider for green colour on LED strip
{
  greenPinValue = param.asInt();
  greenLightAmount = greenPinValue;
 
  if (greenPinValue = 1) {
    white_led();
  }
}

BLYNK_WRITE(V10)
//Adjusting slider for blue colour on LED strip
{
  int bluePinValue = param.asInt();
  blueLightAmount = bluePinValue;

  if (bluePinValue = 1) {
    white_led();
  }    
}

BLYNK_WRITE(V12) {
  int pinValue = param.asInt();

  if (pinValue > 0) {
    reset_everything();
  }
}
BLYNK_WRITE(V18) {
  int pinValue = param.asInt();

  if (pinValue > 0) {
    digitalWrite(D8, HIGH);
  }
  else {
    digitalWrite(D8, LOW);
  }
}

BLYNK_WRITE(V15) {
  int pinValue = param.asInt();

  if (pinValue > 0) {
    digitalWrite(D5, HIGH);
  }
  else {
    digitalWrite(D5, LOW);
  }
}

BLYNK_WRITE(V17) {
  int pinValue = param.asInt();

  if (pinValue > 0) {
    digitalWrite(D7, HIGH);
  }
  else {
    digitalWrite(D7, LOW);
  }
}

BLYNK_WRITE(V16) {
  int pinValue = param.asInt();

  if (pinValue > 0) {
    digitalWrite(D6, HIGH);
  }
  else {
    digitalWrite(D6, LOW);
  }
}

DHT dht_3(D9,11);
void Event1()
{Blynk.virtualWrite(V6, dht_3.readTemperature());}
void Event2()
{Blynk.virtualWrite(V5, dht_3.readHumidity());}

void setup()
{
  // Debug console
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, PIN1, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  
  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht_3.begin();
  timer.setInterval(5000L, Event1);
  timer.setInterval(5000L, Event2);
  
  digitalWrite(D3,HIGH);

  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D8,OUTPUT);

  reset_everything();

}

void reset_everything() {
  
  Blynk.virtualWrite(V15,LOW);
  digitalWrite(D5,LOW);
  Blynk.virtualWrite(V16,LOW);
  digitalWrite(D6,LOW);
  Blynk.virtualWrite(V17,LOW);
  digitalWrite(D7,LOW);
  Blynk.virtualWrite(V18,LOW);  
  digitalWrite(D8,LOW);

  minTempValue = 30; //The default minimum temp is set to 30 degrees upon startup
  Blynk.virtualWrite(V0,minTempValue); //Output default minimum temp to V0

  autoFanValue = 0;
  Blynk.virtualWrite(V1,autoFanValue);
  Blynk.virtualWrite(V2,buttonOffMsg);

  Blynk.virtualWrite(V8,0);
  Blynk.virtualWrite(V9,0);
  Blynk.virtualWrite(V10,0);

  led_off();
}

void white_led() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(redLightAmount,greenLightAmount,blueLightAmount);
    FastLED.show();
  }
}

void led_off() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0,0,0);
    FastLED.show();
  }
}

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
