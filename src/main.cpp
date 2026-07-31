#include <Arduino.h>
#include "dht_task.h"
#include "led_task.h"
#include "mqtt_task.h"

// Deklarasi global handle
QueueHandle_t dhtQueue;
SemaphoreHandle_t buttonSemaphore;

void setup() {
  Serial.begin(115200);

  // Create Queue & Semaphore
  dhtQueue = xQueueCreate(5, sizeof(SensorData));
  buttonSemaphore = xSemaphoreCreateBinary();

  // Membuka Task FreeRTOS pada Core 1
  xTaskCreatePinnedToCore(TaskReadDHT, "DHT Task", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskControlLED, "LED Task", 2048, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskMQTT, "MQTT Task", 4096, NULL, 1, NULL, 1);

  Serial.println("FreeRTOS Tasks Initialized Successfully!");
}

void loop() {
  // Kosongkan superloop karena semua dijalankan via Task RTOS
  vTaskDelay(pdMS_TO_TICKS(1000));
}
