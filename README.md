# Smart Door Lock IoT (ESP32 + RFID + Firebase)

Sistem pengunci pintu otomatis berbasis Internet of Things (IoT) menggunakan ESP32 dan RFID untuk autentikasi akses, serta terintegrasi dengan Firebase untuk monitoring dan kontrol secara real-time melalui web dashboard.

---

## Fitur Utama
- Autentikasi akses menggunakan RFID (UID Card)
- Monitoring dan kontrol real-time berbasis Firebase
- Pencatatan riwayat akses pengguna (log system)
- Mode offline terbatas untuk admin (fail-safe system)
- Notifikasi buzzer dan tampilan LCD I2C
- Indikator LED untuk status akses

---

## Teknologi yang Digunakan

### Hardware
- ESP32  
- RFID RC522  
- Relay Module  
- LCD 16x2 I2C  
- Buzzer  
- LED Indicator  
- Power Supply + Battery Backup (UPS)

### Software
- Arduino IDE  
- Firebase Realtime Database  
- Web Dashboard (HTML, CSS, JavaScript)

### Protocol
- WiFi Communication  
- HTTP / Firebase API  
- I2C (LCD)  
- SPI (RFID)

---

## Cara Kerja Sistem
1. Pengguna menempelkan kartu RFID ke sensor  
2. ESP32 membaca UID dari kartu  
3. UID dikirim ke Firebase untuk verifikasi  
4. Jika terdaftar:
   - Relay aktif (pintu terbuka)
   - Data akses dicatat ke database  
5. Jika tidak terdaftar:
   - Akses ditolak
   - Buzzer berbunyi  
6. Jika offline:
   - Hanya UID admin yang bisa mengakses  
