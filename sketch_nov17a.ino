
#include <WiFi.h>

// =====================================
// 1. KONFIGURASI PIN SENSOR
// =====================================
const int PIR_PIN = 27;
const int LDR_PIN = 34;

// =====================================
// 2. KONFIGURASI WIFI
// =====================================
const char* ssid = "MR-FK-01";
const char* password = "adzanmasjid";

// =====================================
// 3. PENGATURAN SENSOR LDR
// =====================================
int batasCahaya = 2000;

// false: nilai LDR meningkat ketika gelap
// true : nilai LDR menurun ketika gelap
bool nilaiTurunSaatGelap = false;

// =====================================
// 4. PENGATURAN WIFI
// =====================================
unsigned long waktuCobaWiFi = 0;
const unsigned long intervalWiFi = 10000;

// =====================================
// 5. SETUP
// =====================================
void setup() {

  Serial.begin(115200);

  // Mengatur pin sensor PIR sebagai input
  pinMode(PIR_PIN, INPUT);

  // Mengatur resolusi pembacaan analog ESP32
  analogReadResolution(12);

  Serial.println();
  Serial.println("================================");
  Serial.println(" SMART CLASSROOM ENERGY SAVER ");
  Serial.println("================================");

  // Menghubungkan ESP32 ke WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  int percobaan = 0;

  while (WiFi.status() != WL_CONNECTED && percobaan < 30) {
    delay(500);
    Serial.print(".");
    percobaan++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi berhasil terhubung!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi gagal terhubung!");
    Serial.println("Sensor tetap berjalan tanpa WiFi.");
  }

  // Menunggu sensor PIR stabil
  Serial.println();
  Serial.println("Menunggu sensor PIR stabil...");
  delay(30000);

  Serial.println("Sistem siap digunakan!");
}

// =====================================
// 6. LOOP UTAMA
// =====================================
void loop() {

  // Membaca sensor PIR
  int gerakan = digitalRead(PIR_PIN);

  // Membaca sensor LDR
  int cahaya = analogRead(LDR_PIN);

  // Menentukan kondisi cahaya
  bool gelap;

  if (nilaiTurunSaatGelap) {
    gelap = cahaya < batasCahaya;
  } else {
    gelap = cahaya > batasCahaya;
  }

  // Menentukan status ruangan
  int statusRuangan;

  if (gerakan == HIGH && gelap) {
    statusRuangan = 2;
  }
  else if (gerakan == HIGH && !gelap) {
    statusRuangan = 1;
  }
  else {
    statusRuangan = 0;
  }

  // =================================
  // MENAMPILKAN HASIL SENSOR
  // =================================

  Serial.println("--------------------------------");

  Serial.print("Deteksi Gerakan : ");

  if (gerakan == HIGH) {
    Serial.println("TERDETEKSI");
  } else {
    Serial.println("TIDAK TERDETEKSI");
  }

  Serial.print("Nilai Cahaya    : ");
  Serial.println(cahaya);

  Serial.print("Kondisi Cahaya  : ");

  if (gelap) {
    Serial.println("GELAP");
  } else {
    Serial.println("TERANG");
  }

  Serial.print("Status Ruangan  : ");

  if (statusRuangan == 2) {
    Serial.println("LAMPU DIPERLUKAN");
  }
  else if (statusRuangan == 1) {
    Serial.println("RUANGAN TERANG");
  }
  else {
    Serial.println("TIDAK ADA GERAKAN");
  }

  // =================================
  // MEMERIKSA KONEKSI WIFI
  // =================================

  if (WiFi.status() != WL_CONNECTED) {

    if (millis() - waktuCobaWiFi >= intervalWiFi) {

      waktuCobaWiFi = millis();

      Serial.println("Mencoba menghubungkan WiFi kembali...");

      WiFi.disconnect();
      WiFi.begin(ssid, password);
    }
  }

  // Membaca sensor setiap 1 detik
  delay(1000);
}
