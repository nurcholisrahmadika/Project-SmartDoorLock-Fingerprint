#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

unsigned long lastRestart = 0;
const unsigned long interval = 60000UL;
// ==== OLED ====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==== Fingerprint ====
SoftwareSerial fingerSerial(D7, D8); // RX, TX
Adafruit_Fingerprint finger(&fingerSerial);

// ==== Pin ====
#define RELAY_PIN   D6
#define BUZZER_PIN  D5
#define LED_GREEN   D3
#define LED_RED     D4
#define EXIT_BTN    D0   // tombol keluar (aktif HIGH, pull-down 10kΩ)

void doorOpenedTone() {
  for (int i = 0; i < 2; i++) {
    tone(BUZZER_PIN, 2000, 150);  // bunyi 150ms
    delay(200);                   // jeda antar bunyi
  }
}

void setup() {
    lastRestart = millis();
  // Serial monitor
  Serial.begin(115200);

  // OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED gagal!");
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Inisialisasi...");
  display.display();

  // Pin mode
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
pinMode(EXIT_BTN, INPUT_PULLUP);  // push button aktif HIGH (pakai pull-down resistor 10kΩ)

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);

  // Fingerprint setup
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Sensor fingerprint ditemukan.");
    display.setCursor(0, 16);
    display.println("Fingerprint OK");
    display.display();
  } else {
    Serial.println("Sensor fingerprint TIDAK terdeteksi!");
    display.setCursor(0, 16);
    display.println("Finger Error!");
    display.display();
    while (1) delay(1);
  }

  delay(2000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Siap scan jari");
  display.display();
}

void loop() {
  if (millis() - lastRestart >= interval) {
      ESP.restart();  // Restart ESP32
  }

  // ==== cek tombol keluar ====
  if (digitalRead(EXIT_BTN) == HIGH) {   // aktif HIGH
    aksesManual();
  }

  // ==== cek fingerprint ====
  int id = getFingerprintID();
  if (id > 0) {
    aksesDiterima(id);
  } else if (id == -1) {
    aksesDitolak();
  }
  delay(100);
}

// ==== Fingerprint Check ====
int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return 0;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return 0;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  Serial.print("Ditemukan ID #"); Serial.println(finger.fingerID);
  return finger.fingerID;
}

// ==== Akses diterima ====
void aksesDiterima(int id) {
  Serial.println("Akses Diterima!");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Akses Diterima");
  display.setCursor(0, 16);
  display.print("ID: ");
  display.println(id);
  display.display();

  // LED hijau dan relay aktif
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(RELAY_PIN, HIGH);

  // 🔊 Bunyi positif — dua nada tinggi cepat
  for (int i = 0; i < 2; i++) {
    tone(BUZZER_PIN, 2500, 120);  // nada tinggi (lebih tajam)
    delay(180);
  }

  delay(20000); // waktu pintu terbuka 5 detik

  // Matikan LED dan relay
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  // Kembali ke mode siap
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Siap scan jari");
  display.display();
}


// ==== Akses ditolak ====
void aksesDitolak() {
  Serial.println("Akses Ditolak!");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Akses Ditolak!");
  display.display();

  // LED merah aktif
  digitalWrite(LED_RED, HIGH);

  // 🔊 Bunyi negatif — tiga beep cepat nada rendah
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 800, 150);  // nada rendah
    delay(200);
  }

  delay(5000);

  // Matikan LED merah dan kembali ke mode siap
  digitalWrite(LED_RED, LOW);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Siap scan jari");
  display.display();
}


// ==== Akses manual (tombol keluar) ====
void aksesManual() {
  Serial.println("Tombol keluar ditekan!");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Tombol Keluar");
  display.setCursor(0, 16);
  display.println("Akses Dibuka");
  display.display();

  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(RELAY_PIN, HIGH);
  tone(BUZZER_PIN, 2000, 200);

  delay(20000);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Siap scan jari");
  display.display();
}
