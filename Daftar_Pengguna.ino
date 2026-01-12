#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// ==== OLED ====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==== Fingerprint ====
SoftwareSerial mySerial(D7, D8); // RX=D7, TX=D8 (NodeMCU)
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// ==== Pin I/O ====
#define RELAY_PIN   D5
#define BUZZER_PIN  D6
#define LED_GREEN   D3
#define LED_RED     D4
#define EXIT_BTN    D0   // nanti bisa dipakai untuk hapus semua

void setup() {
  Serial.begin(115200);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED gagal!");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Init Fingerprint...");
  display.display();

  // Pin setup
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(EXIT_BTN, INPUT_PULLUP); // tombol pakai pull-up

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);

  // Finger init
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Sensor fingerprint OK");
    display.setCursor(0, 16);
    display.println("Fingerprint OK");
    display.display();
  } else {
    Serial.println("Sensor fingerprint ERROR!");
    display.setCursor(0, 16);
    display.println("Finger ERROR!");
    display.display();
    while (1);
  }

  delay(2000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Siap daftar jari");
  display.display();
}

void loop() {
  int id = getEmptyID();
  if (id == -1) {
    Serial.println("Database penuh!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("DB penuh (127)");
    display.display();

    // gagal → buzzer panjang + LED merah
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_RED, LOW);

    while (1); // stop kalau penuh
  }

  Serial.print("Pendaftaran ID #");
  Serial.println(id);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Daftar jari baru");
  display.setCursor(0, 16);
  display.print("ID: ");
  display.println(id);
  display.display();

  if (enrollFingerprint(id)) {
    Serial.print("Berhasil daftar ID #");
    Serial.println(id);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Berhasil daftar");
    display.setCursor(0, 16);
    display.print("ID: ");
    display.println(id);
    display.display();

    // sukses → LED hijau + buzzer 2x
    digitalWrite(LED_GREEN, HIGH);
    beep(2, 150, 150);
    digitalWrite(LED_GREEN, LOW);
  } else {
    Serial.print("Gagal daftar ID #");
    Serial.println(id);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Gagal daftar");
    display.setCursor(0, 16);
    display.print("ID: ");
    display.println(id);
    display.display();

    // gagal → buzzer panjang + LED merah
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_RED, LOW);
  }

  delay(3000); // tunggu sebelum daftar ID berikutnya
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Siap daftar jari");
  display.display();
}

bool enrollFingerprint(int id) {
  int p = -1;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Tempel jari 1x");
  display.display();

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) return false;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Lepaskan jari...");
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Tempel jari 2x");
  display.display();

  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) return false;

  p = finger.createModel();
  if (p != FINGERPRINT_OK) return false;

  p = finger.storeModel(id);
  return (p == FINGERPRINT_OK);
}

int getEmptyID() {
  for (int i = 1; i <= 127; i++) {
    if (finger.loadModel(i) != FINGERPRINT_OK) {
      return i;
    }
  }
  return -1; // penuh
}

// fungsi beep untuk buzzer
void beep(int times, int onDelay, int offDelay) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(onDelay);
    digitalWrite(BUZZER_PIN, LOW);
    delay(offDelay);
  }
}
