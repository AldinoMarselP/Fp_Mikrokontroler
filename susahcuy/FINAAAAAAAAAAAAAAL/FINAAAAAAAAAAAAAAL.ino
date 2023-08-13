
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

BlynkTimer timer;

char auth[] = "uu6OYodPCh4Wr9tC_9HS0605swcFKJpK";

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

BLYNK_WRITE(V6)
{
  autoFanValue = param.asInt();
}

BLYNK_WRITE(V7)
{
  buttonOffMsg = param.asString();
  fanOnMsg = param.asString();
  fanOffMsg = param.asString();
}

BLYNK_WRITE(V1) // Menghidupkan Lampu Tengah
{
  int pinValue = param.asInt();

  if (pinValue == 1) {
    Serial.println("Lampu Tengah : ON");
    digitalWrite(D7, HIGH);
  }
  else {
    Serial.println("Lampu Tengah : OFF");
    digitalWrite(D7, LOW);
  }
}

BLYNK_WRITE(V2)  // Menghidupkan Lampu Dapur
{
  int pinValue = param.asInt();
  if (pinValue == 1) {
    Serial.println("Lampu Dapur : ON");
    digitalWrite(D8, HIGH);
  }
  else {
    Serial.println("Lampu Dapur : OFF");
    digitalWrite(D8, LOW);
  }
}

BLYNK_WRITE(V3) // Menghidupkan Lampu Kamar
{
  int pinValue = param.asInt();
  if (pinValue == 1) {
    Serial.println("Lampu Kamar : ON");
    digitalWrite(D6, HIGH);
  }
  else {
    Serial.println("Lampu Kamar : OFF");
    digitalWrite(D6, LOW );
  }
}

DHT dht (D2, DHT11);    // Inisialisasi Sensor DHT11
void Event1()           // Menampilkan Suhu pada Blynk
{
  Blynk.virtualWrite(V4, dht.readTemperature());
}
void Event2()           // Menampilkan Kelembaban pada Blynk
{
  Blynk.virtualWrite(V5, dht.readHumidity());
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  dht.begin();          // memulai sensor dht11
  timer.setInterval(5000L, Event1);   //Untuk timer intrval Suhu pada Blynk
  timer.setInterval(5000L, Event2);   //Untuk timer intrval Kelembaban pada Blynk

  Blynk.begin(auth, ssid, pass, IPAddress(43, 229, 135, 169), 8080);    // Memulai Blynk
  pinMode(D6, OUTPUT);  // Inisialisasi LED
  pinMode(D7, OUTPUT);  // Inisialisasi LED
  pinMode(D8, OUTPUT);  // Inisialisasi LED

}


void loop()
{
  Blynk.run();
  timer.run();

  if (autoFanValue > 0)
  {
    if (dht.readTemperature() > minTempValue) {
      digitalWrite(D1, HIGH);
      Blynk.virtualWrite(V7, fanOnMsg);
    }

    if (dht.readTemperature() < minTempValue) {
      digitalWrite(D1, LOW);
      Blynk.virtualWrite(V7, fanOffMsg);
    }
  }
  else {
    digitalWrite(D1, LOW);
    Blynk.virtualWrite(V7, buttonOffMsg);
  }
}
