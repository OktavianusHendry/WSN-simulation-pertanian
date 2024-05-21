// Blynk Template and Auth Key
#define BLYNK_TEMPLATE_ID "TMPL67MUZRyVN"
#define BLYNK_TEMPLATE_NAME "WSN Temp Light Mois"
#define BLYNK_AUTH_TOKEN "t3-jlcDM-N9DUBB8WSXwwK-jMaX1Nlxf"
// Library yang digunakan
#include <Adafruit_Sensor.h>
#include <SimpleDHT.h>
#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
// Pin ESP32 yang digunakan untuk Soil Moisture dan Light Dependent Resistor
#define PIN_SOIL 4
#define PIN_LIGHT 5
// Pin ESP32 yang digunakan untuk DHT11 dan inisialisasi jenis DHT11
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

// Koneksi Blynk dan WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "LAPTOP-KLE019TO 9118";  //nama WiFi
char pass[] = "7N&6x010"; //password WiFi

// Proses yang dijalankan ketika sistem pertama kali menyala
void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

// Proses yang terulang terus
void loop() {
  // start working...
  Blynk.run();  //run blynk
  Serial.println("=================================");
  Serial.println("Smart Agriculture...");

  //Soil Moisture
  int value = analogRead(PIN_SOIL);  // membaca nilai analog dari pin
  int valuePercentage = (100 - ((value / 4095.00) * 100)); // Mengubah nilai analog ke persen
  Blynk.virtualWrite(V1, valuePercentage); // Menulis nilai persen ke Blynk

  //Pengecekan kondisi tanah
  if (value >= 2496) { // 0 - 39%
    Serial.println("Kelembaban tanah: KERING");
    Blynk.virtualWrite(V5, "Kering");
    Blynk.logEvent("soil_dry"); // mengirim notifikasi email dan app mengenai kelembaban tanah terlalu kering
  } else if (value >= 1600 && value <= 2495) { // 40 - 60%
    Serial.println("Kelembaban tanah: OPTIMAL");
    Blynk.virtualWrite(V5, "Optimal");
  } else { // 61 - 100%
    Serial.println("Kelembaban tanah: TERLALU BASAH");
    Blynk.virtualWrite(V5, "Terlalu Basah");
    Blynk.logEvent("soil_too_wet"); // mengirim notifikasi email dan app mengenai kelembaban tanah terlalu basah
  }
  Serial.print(value);
  Serial.print(" or ");
  Serial.print(valuePercentage);
  Serial.print("%");
  Serial.println(")");
  delay(500);

  //LDR Sensor
  int lightValue = analogRead(PIN_LIGHT);
  int invertedLightValue = 4095 - lightValue;               
  float Vout = float(invertedLightValue) * (5.0 / 4095.0);  // Konversi dari inverted analog ke voltase
  float RLDR = (10000.0 * (5.0 - Vout)) / Vout;             // Konversi dari voltase ke resistansi
  int lux = 500 / (RLDR / 1000);                            // Konversi dari resistansi ke lux

  Blynk.virtualWrite(V0, lux); // Menulis nilai lux ke Blynk
  Serial.print("Light Value = ");
  Serial.print(lightValue);
  // Pengecekan kondisi dari pencahayaan
  if (invertedLightValue < 40) {
    Serial.println(" => Dark");
    Blynk.virtualWrite(V4, "Dark");
  } else if (invertedLightValue < 800) {
    Serial.println(" => Dim");
    Blynk.virtualWrite(V4, "Dim");
  } else if (invertedLightValue < 2000) {
    Serial.println(" => Light");
    Blynk.virtualWrite(V4, "Light");
  } else if (invertedLightValue < 3200) {
    Serial.println(" => Bright");
    Blynk.virtualWrite(V4, "Bright");
  } else {
    Serial.println(" => Very bright");
    Blynk.virtualWrite(V4, "Very bright");
  }
  delay(1000);

  //DHT11
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess; // untuk mengeluarkan pesan error
  // Pengecekan apakah DHT11 mengalami error, jika iya maka print pesan error
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.print(SimpleDHTErrCode(err));
    Serial.print(",");
    Serial.println(SimpleDHTErrDuration(err));
    delay(1000);
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); // print nilai temperatur
  Serial.println(" *C");

  //Pengecekan kondisi suhu udara untuk Stroberi
  if (temperature < 22){
    Blynk.virtualWrite(V3, "Too Cold");
    Blynk.logEvent("temperature_low"); // mengirim notifikasi email dan app mengenai suhu terlalu rendah
  } else if (temperature <= 28 && temperature >= 22){
    Blynk.virtualWrite(V3, "Normal");
  } else{
    Blynk.virtualWrite(V3, "Too Hot");
    Blynk.logEvent("temperature_high"); // mengirim notifikasi email dan app mengenai suhu terlalu tinggi
  }
  Blynk.virtualWrite(V2, temperature); // Mengirim nilai suhu ke Blynk

  delay(1500);
}