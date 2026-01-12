#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#include <HardwareSerial.h>
HardwareSerial mySerial(2); // RX2 = 16, TX2 = 17 di ESP32
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// === Pin ===
#define BUTTON_PIN 0   // Tombol di D0
#define BUZZER_PIN 6   // Buzzer di D6
#define LED_GREEN  3   // LED hijau di D3
#define LED_RED    4   // LED merah di D4

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);

  Serial.begin(115200);
  mySerial.begin(57600, SERIAL_8N1, 16, 17);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // OLED gagal
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Init Fingerprint...");
  display.display();

  if (finger.verifyPassword()) {
    display.setCursor(0, 10);
    display.println("Sensor terdeteksi");
  } else {
    display.setCursor(0, 10);
    display.println("Sensor tidak ada!");
    display.display();
    while (1) delay(1);
  }
  display.display();
  delay(1000);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    unsigned long startTime = millis();
    while (digitalRead(BUTTON_PIN) == HIGH) {
      if (millis() - startTime >= 5000) {  // Tekan 5 detik
        hapusSemuaSidik();
        break;
      }
    }
  }
}

void hapusSemuaSidik() {
  // Bunyi panjang saat mulai hapus
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Menghapus semua...");
  display.display();

  uint8_t p = finger.emptyDatabase();
  display.setCursor(0, 10);

  if (p == FINGERPRINT_OK) {
    display.println("Berhasil dihapus!");
    display.display();
    indikatorBerhasil();
  } else {
    display.println("Gagal hapus!");
    display.display();
    indikatorGagal();
  }

  delay(2000);
}

// === Indikator berhasil ===
void indikatorBerhasil() {
  // Buzzer bunyi panjang sekali
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);

  // LED merah ON
  digitalWrite(LED_RED, HIGH);

  // LED hijau kedip 5x
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_GREEN, HIGH);
    delay(200);
    digitalWrite(LED_GREEN, LOW);
    delay(200);
  }

  digitalWrite(LED_RED, LOW);
}

// === Indikator gagal ===
void indikatorGagal() {
  // Buzzer 3x pendek
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }

  // LED merah kedip 3x
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_RED, HIGH);
    delay(200);
    digitalWrite(LED_RED, LOW);
    delay(200);
  }
}
