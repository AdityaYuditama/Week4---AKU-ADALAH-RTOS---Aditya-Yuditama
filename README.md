# Week 4 IoT - Real-Time Operating System (RTOS)

**Nama**    : Aditya Yuditama  
**NIM**     : 101012400311
**Divisi**  : IoT

Proyek ini merupakan implementasi sistem pemantauan lingkungan berbasis **FreeRTOS** pada ESP32 menggunakan framework Arduino & PlatformIO di Wokwi.

---

## 🛠️ Fitur & Arsitektur RTOS

Sistem ini menjalankan 3 Task utama secara simultan pada ESP32:

1. **`TaskReadDHT` (Sensor Reader)**
   - Membaca data suhu dan kelembaban dari sensor **DHT22** secara berkala (2 detik).
   - Mengirimkan data pembacaan ke `TaskMQTT` menggunakan **FreeRTOS Queue** (`dhtQueue`).

2. **`TaskMQTT` (Cloud Publisher)**
   - Mengelola koneksi Wi-Fi (`Wokwi-GUEST`) dan MQTT Broker (`broker.hivemq.com`).
   - Menerima data dari Queue dan mempublikasikannya ke topik `cps/rtos/dht22`.

3. **`TaskControlLED` (Interrupt & Hardware Control)**
   - Menangani **Hardware Interrupt (ISR)** dari Push Button pada PIN 14.
   - Menggunakan **Binary Semaphore** (`buttonSemaphore`) untuk melakukan *deferred processing* dan mengubah status indikator LED Merah/Hijau secara instan.

---

## 🔌 Pinout & Hardware

- **DHT22 Data Pin**: GPIO 15
- **Push Button Pin**: GPIO 14 (Interrupt RISING)
- **LED Red**: GPIO 4
- **LED Green**: GPIO 2

---

## 🚀 Cara Menjalankan Simulasi

1. Clone repositori ini:
   ```bash
   git clone <https://github.com/AdityaYuditama/Week4---AKU-ADALAH-RTOS---Aditya-Yuditama>