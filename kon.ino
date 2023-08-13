
#define BLYNK_PRINT Serial      //Untuk menampilkan Pesan dari Blynk jika berhasil terhubung pada serial monitor
#include <ESP8266WiFi.h>        //modul nodemcu
#include <BlynkSimpleEsp8266.h> //Agar bisa terbaca di aplikasi Blynk
#include <DHT.h>                //sensor suhu

BlynkTimer timer;               //Library untuk timer pada Blynk

char auth[] = "uu6OYodPCh4Wr9tC_9HS0605swcFKJpK"; //Kode autentifikasi
char ssid[] = "POCO";                             //Nama hotspot
char pass[] = "12345678";                         //Password hotspot

int minTempValue;             //Deklarasi untuk minimal temperatur
int autoFanValue;             //Deklarasi nilai otomatisasi kipas

String buttonOffMsg = "Automatic Fan System is disabled.";  //Deklarasi nilai button message
String fanOnMsg = "Fan Status: ON";                         //Deklarasi nilai ketika kipas menyala
String fanOffMsg = "Fan Status: OFF";                       //Deklarasi nilai ketika kipas mati

BLYNK_WRITE(V0)      //Mendeklarasikan minimal suhu pada pin Virtual 0 (sesuai pada Blynk App)
{
  minTempValue = param.asInt(); //Membuat variabel menjadi int pada Blynk
}

BLYNK_WRITE(V6)     //Mendeklarasikan nilai otomatisasi kipas pada pin Virtual 6 (sesuai pada Blynk App)
{
  autoFanValue = param.asInt(); //Membuat variabel menjadi int pada Blynk
}

BLYNK_WRITE(V7)     //Mendeklarasikan kondisi kipas pada pin Virtual 7 (sesuai pada Blynk App)
{
  buttonOffMsg = param.asString();  //Membuat variabel menjadi String pada Blynk
  fanOnMsg = param.asString();      //Membuat variabel menjadi String pada Blynk
  fanOffMsg = param.asString();     //Membuat variabel menjadi String pada Blynk
}

BLYNK_WRITE(V1)     // Mendeklarasikan button lampu tengah pada pin Virtual 1 (sesuai pada Blynk App)
{
  int pinValue = param.asInt();           //Membuat variabel menjadi int pada Blynk

  if (pinValue == 1)                      //Kondisi percabangan untuk lampu On atau Off
  { 
    Serial.println("Lampu Tengah : ON");  //Menampilkan output pada serial print
    digitalWrite(D7, HIGH);               //Menghidupkan lampu
  }
  else 
  {
    Serial.println("Lampu Tengah : OFF"); //Menampilkan output pada serial print
    digitalWrite(D7, LOW);                //Mematikan lampu
  }
}

BLYNK_WRITE(V2)  // Mendeklarasikan button lampu dapur pada pin Virtual 2 (sesuai pada Blynk App)
{
  int pinValue = param.asInt();   //Membuat variabel menjadi int pada Blynk
  
  if (pinValue == 1) {            //Kondisi percabangan untuk lampu On atau Off
    Serial.println("Lampu Dapur : ON");   //Menampilkan output pada serial print
    digitalWrite(D8, HIGH);               //Menghidupkan lampu
  }
  else {
    Serial.println("Lampu Dapur : OFF");  //Menampilkan output pada serial print
    digitalWrite(D8, LOW);                //Mematikan lampu
  }
}

BLYNK_WRITE(V3) // Mendeklarasikan button lampu kamar pada pin Virtual 3 (sesuai pada Blynk App)
{
  int pinValue = param.asInt(); //Membuat variabel menjadi int pada Blynk
  
  if (pinValue == 1) {          //Kondisi percabangan untuk lampu On atau Off
    Serial.println("Lampu Kamar : ON");  //Menampilkan output pada serial print
    digitalWrite(D6, HIGH);              //Menghidupkan lampu
  }
  else {
    Serial.println("Lampu Kamar : OFF");  //Menampilkan output pada serial print
    digitalWrite(D6, LOW );               //Menghidupkan lampu
  }
}

DHT dht (D2, DHT11);    // Inisialisasi Sensor DHT11 (pada pin D2)

void BacaSuhu()           // Menampilkan Suhu pada Blynk
{
  Blynk.virtualWrite(V4, dht.readTemperature()); //Menampilkan output pada Blynk berupa hasil pembacaan suhu
}

void BacaLembab()           // Menampilkan Kelembaban pada Blynk
{
  Blynk.virtualWrite(V5, dht.readHumidity()); //Menampilkan output pada Blynk berupa hasil pembacaan kelembaban
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  dht.begin();          // memulai sensor dht11
  timer.setInterval(5000L, BacaSuhu);     //Untuk timer intrval Suhu pada Blynk selama 5 detik
  timer.setInterval(5000L, BacaLembab);   //Untuk timer intrval Kelembaban pada Blynk selama 5 detik

  Blynk.begin(auth, ssid, pass, IPAddress(43, 229, 135, 169), 8080);    // Memulai Blynk
  pinMode(D6, OUTPUT);  // Inisialisasi LED pada pin D6
  pinMode(D7, OUTPUT);  // Inisialisasi LED pada pin D7
  pinMode(D8, OUTPUT);  // Inisialisasi LED pada pin D8

}


void loop()
{
  Blynk.run(); //inisialisasi untuk menghidupkan/menyambungkan blynk
  timer.run(); //finisialisasi untuk mengaktifkan fungsi timer pada Blynk

  if (autoFanValue > 0)   //Kondisi percabangan untuk Kipas On atau Off bergantung pada min suhu
  {
    if (dht.readTemperature() > minTempValue) { //Kondisi jika suhu lebih dari minimal suhu
      digitalWrite(D1, HIGH);                   //Kipas menyala
      Blynk.virtualWrite(V7, fanOnMsg);         //Menampilkan output pada Blynk sesuai nilai dari variabel 
    }

    if (dht.readTemperature() < minTempValue) { //Kondisi jika suhu kurang dari minimal suhu
      digitalWrite(D1, LOW);                    //Kipas mati
      Blynk.virtualWrite(V7, fanOffMsg);        //Menampilkan output pada Blynk sesuai nilai dari variabel 
    }
  }
  else {                                        //Kondisi jika button off pada Blynk ditekan
    digitalWrite(D1, LOW);                      //Kipas mati
    Blynk.virtualWrite(V7, buttonOffMsg);       //Menampilkan output pada Blynk sesuai nilai dari variabel 
  }
}
