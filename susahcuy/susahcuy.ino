#include <DHT.h>
#include <Blynk.h>
#define DHTPIN D2
#define DHTTYPE DHT11
#define RELAY D4


DHT dht(DHTPIN, DHTTYPE);

int pin1 = D6;
int pin2 = D7;
int pin3 = D8;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);

  pinMode(RELAY, OUTPUT);
}

void loop() {
  delay(2000);
  float temperature = dht.readTemperature();

  if (temperature > 30) {
    digitalWrite(RELAY, HIGH);
    Serial.println("The fan is turned on");

  } else {
    digitalWrite(RELAY, LOW);
    Serial.println("The fan is turned off");
  }

  Serial.print("Temperature: ");
  Serial.println(temperature);

  digitalWrite(pin1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(pin2, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(pin3, HIGH);   // turn the LED on (HIGH is the voltage level)
}
