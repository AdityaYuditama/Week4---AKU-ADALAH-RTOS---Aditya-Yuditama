#ifndef DHT_TASK_H
#define DHT_TASK_H

#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

// Struktur data untuk menampung pembacaan sensor
struct SensorData {
  float temperature;
  float humidity;
};

// Queue Handle untuk passing data antar-task
extern QueueHandle_t dhtQueue;

void TaskReadDHT(void *pvParameters) {
  DHT dht(DHTPIN, DHTTYPE);
  dht.begin();
  
  SensorData data;

  for (;;) {
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();

    if (!isnan(data.temperature) && !isnan(data.humidity)) {
      // Kirim data ke Queue agar bisa dibaca oleh MQTT Task
      xQueueSend(dhtQueue, &data, portMAX_DELAY);
    }

    // Delay khas FreeRTOS (bukan delay() biasa)
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

#endif