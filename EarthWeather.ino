#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library untuk LCD I2C
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "DHT.h"

// Definisi pin
#define soilMoisture A0
#define DHTPIN D3
#define DHTTYPE DHT11
#define trigPin D8
#define echoPin D7
#define relayPump_1 D4
#define relayPump_2 D5
#define relayFan D6

// Konstanta WiFi dan Firebase
const char *ssid = "Kos Biru";
const char *password = "Bagyo123";
const char *serverName = "https://ibnux.github.io/BMKG-importer/cuaca/5002229.json";
const char *host = "192.168.1.6";

// Objek global
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
DynamicJsonDocument doc(1536);

// Variabel global
int valueMoisture;
int kelembaban;
int suhu;
long duration;
int distance;
int relayStateON = LOW;
int relayStateOFF = HIGH;
unsigned long lastTimeWeather = 0;
unsigned long timerDelayWeather = 10000;
unsigned long lastTimeSensor = 0;
unsigned long timerDelaySensor = 1250;
unsigned long lastTimeServer = 0;
unsigned long timerDelayServer = 1000;
String weatherReading;
String link;
String cuacaJam12;
String kodeCuacaJam12;
bool akanHujanJam12 = false; // Menyimpan apakah akan hujan pada jam 12

// Inisialisasi NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000); // Zona waktu UTC+7 (25200 detik)

// Fungsi untuk menampilkan data pada LCD
void lcdDisplay(int row, int col, String text) {
  lcd.setCursor(col, row);
  lcd.print(text);
}

// Fungsi untuk melakukan request ke API JSON
String httpGETRequest(const char *serverName) {
  WiFiClientSecure httpsClient;
  HTTPClient http;
  String payload;

  // Insecure mode untuk koneksi SSL, hindari di produksi
  httpsClient.setInsecure();

  http.begin(httpsClient, serverName);
  http.setTimeout(10000); // Timeout 10 detik

  int response = http.GET();
  if (response == HTTP_CODE_OK) {
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(response);
  }

  http.end();
  return payload;
}

void readSensors() {
  // Membaca data sensor soil moisture
  valueMoisture = analogRead(soilMoisture);
  valueMoisture = map(valueMoisture, 0, 1024, 0, 100);
  valueMoisture = (valueMoisture - 100) * -1;

  Serial.print("Moisture : ");
  Serial.println(valueMoisture);

  // Membaca data sensor DHT11
  kelembaban = dht.readHumidity();
  suhu = dht.readTemperature();

  Serial.print("Kelembaban : ");
  Serial.println(kelembaban);
  Serial.print("Suhu : ");
  Serial.println(suhu);

  // Membaca data ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);
}

// Fungsi untuk mengupdate status relay berdasarkan sensor
void updateRelayStatus() {
  if (distance >= 10 && distance <= 12) {
    digitalWrite(relayPump_2, relayStateON);
    lcdDisplay(1, 14, "P2: ON");
    Serial.println("P2 : ON ");
  } else if (distance == 5) {
    digitalWrite(relayPump_2, relayStateOFF);
    lcdDisplay(1, 14, "P2: OF");
    Serial.println("P2 : OFF ");
  } else {
    digitalWrite(relayPump_2, relayStateOFF);
    lcdDisplay(1, 14, "P2: OF");
    Serial.println("P2 : OFF ");
  }

  // Default: Matikan relay
  digitalWrite(relayPump_1, relayStateOFF);
  lcdDisplay(1, 7, "P1: OF");
  Serial.println("P1 : OFF ");

  // Ambil jam saat ini dari waktu sistem
  int currentHour = hour();

  // Cek apakah saat ini adalah satu jam sebelum jam 12 (jam 11)
  if (currentHour == 11 && akanHujanJam12) {
    Serial.println("Peringatan: Akan hujan pada jam 12, penyiraman dibatalkan.");
    return; // Batalkan penyiraman
  }

  // Cek waktu penyiraman (antara jam 10 hingga 14)
  if (currentHour >= 10 && currentHour < 14) {
    // Cek apakah cuaca pada jam 12 mengindikasikan hujan
    if (akanHujanJam12) {
      Serial.println("Cuaca pada jam 12 mengindikasikan hujan, penyiraman dibatalkan.");
      return; // Batalkan penyiraman
    }

    // Cek nilai moisture dan kode cuaca untuk penyiraman
    if (valueMoisture < 60 && !akanHujanJam12) {
      digitalWrite(relayPump_2, relayStateON);
      lcdDisplay(1, 7, "P1: ON");
      Serial.println("P1 : ON ");
    }
  }
}

// Fungsi untuk melakukan request ke API cuaca
void fetchWeatherData() {
  weatherReading = httpGETRequest(serverName);
  Serial.println("---- REQUEST RESULT FROM API ----");
  Serial.println(weatherReading);

  deserializeJson(doc, weatherReading);
  JsonArray items = doc.as<JsonArray>();

  for (int i = 0; i < items.size(); i++) {
    JsonObject item = items[i];
    const char *cuaca = item["cuaca"];
    const char *jamCuaca = item["jamCuaca"];
    const char *kodeCuaca = item["kodeCuaca"];

    // Cek apakah jamCuaca adalah jam 12
    if (String(jamCuaca) == "12:00") {
      cuacaJam12 = cuaca;
      kodeCuacaJam12 = kodeCuaca;

      // Jika kode cuaca menunjukkan kemungkinan hujan
      akanHujanJam12 = (String(kodeCuaca) == "60" || String(kodeCuaca) == "61" ||
                        String(kodeCuaca) == "63" || String(kodeCuaca) == "80" ||
                        String(kodeCuaca) == "95" || String(kodeCuaca) == "97");
    }

    // Display cuaca dan jamCuaca
    lcdDisplay(2, 0, cuaca);
    lcdDisplay(3, 0, String(jamCuaca));
    delay(1500);
  }
}

// Fungsi untuk mengirim data ke server kita
void sendDataToServer() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    link = "http://" + String(host) + "/SKRIPSI/kirimdata.php?suhu=" + String(suhu) + "&kelembaban=" + String(kelembaban) + "&mois=" + String(valueMoisture) + "&jarak=" + String(distance);

    http.begin(client, link);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(D2, D1);
  dht.begin();

  pinMode(relayPump_1, OUTPUT);
  pinMode(relayPump_2, OUTPUT);
  pinMode(relayFan, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(relayPump_1, relayStateOFF);
  digitalWrite(relayPump_2, relayStateOFF);
  digitalWrite(relayFan, relayStateOFF);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Inisialisasi NTP
  timeClient.begin();
  timeClient.update();

  // Cetak waktu saat ini untuk verifikasi
  Serial.print("Current time: ");
  Serial.println(timeClient.getFormattedTime());

  lcd.init();
  lcd.backlight();
  lcdDisplay(0, 0, "  Initializing  ");
  for (int a = 5; a <= 10; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();
}

void loop() {
  timeClient.update(); // Update waktu dari NTP

  if ((millis() - lastTimeWeather) > timerDelayWeather) {
    fetchWeatherData();
    lastTimeWeather = millis();
  }

  if ((millis() - lastTimeSensor) > timerDelaySensor) {
    readSensors();
    updateRelayStatus();
    lastTimeSensor = millis();
  }

  if ((millis() - lastTimeServer) > timerDelayServer) {
    sendDataToServer();
    lastTimeServer = millis();
  }

  // Update LCD
  lcdDisplay(0, 0, "Te: " + String(suhu));
  lcdDisplay(0, 7, "Hu: " + String(kelembaban));
  lcdDisplay(0, 14, "Mo: " + String(valueMoisture));
  lcdDisplay(1, 0, "Di: " + String(distance));
  lcdDisplay(1, 7,  "P1: OF");
  lcdDisplay(1, 14, "P2: OF");
}
