# Smart Door Lock Fingerprint (ESP32)

Sistem pengunci pintu otomatis berbasis Internet of Things (IoT) menggunakan sensor fingerprint sebagai metode autentikasi utama. Sistem ini dilengkapi dengan OLED display, buzzer, LED indikator, serta tombol manual untuk membuka pintu dari dalam.

---

## Deskripsi Sistem
Sistem ini dirancang untuk mengontrol akses pintu menggunakan sidik jari. Pengguna yang terdaftar dapat membuka pintu, sedangkan akses yang tidak dikenali akan ditolak. Sistem juga menyediakan tombol keluar (exit button) untuk membuka pintu secara manual dari dalam.

---

## Fitur Utama
- Autentikasi akses menggunakan fingerprint
- Tampilan status pada OLED display
- Indikator LED untuk status akses (diterima / ditolak)
- Notifikasi buzzer untuk feedback suara
- Relay untuk kontrol kunci pintu
- Tombol manual (exit button) untuk akses dari dalam
- Sistem auto-restart untuk menjaga stabilitas

---

## Teknologi yang Digunakan

### Hardware
- ESP32  
- Sensor Fingerprint (Adafruit compatible)  
- OLED SSD1306 (I2C)  
- Relay Module  
- Buzzer  
- LED Indicator  
- Push Button (Exit Button)  

### Software
- Arduino IDE  
- Adafruit Fingerprint Library  
- Adafruit SSD1306  
- Adafruit GFX  

### Protocol
- I2C (OLED)  
- Serial Communication (Fingerprint)  

---

## Cara Kerja Sistem
1. Sistem menunggu input sidik jari  
2. Sensor membaca fingerprint pengguna  
3. Jika data cocok:
   - Pintu terbuka (relay aktif)
   - LED hijau menyala
   - Buzzer memberikan suara indikator
4. Jika tidak cocok:
   - Akses ditolak
   - LED merah menyala
   - Buzzer berbunyi sebagai peringatan
5. Tombol exit dapat digunakan untuk membuka pintu dari dalam  
6. Sistem akan restart otomatis setiap interval untuk menjaga kestabilan  

---

## Struktur Project
