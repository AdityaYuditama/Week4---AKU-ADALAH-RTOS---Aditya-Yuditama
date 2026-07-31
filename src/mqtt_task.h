#ifndef MQTT_TASK_H
#define MQTT_TASK_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "dht_task.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com"; // Broker publik gratis

WiFiClient espClient;
PubSubClient client(espClient);

void TaskMQTT(void *pvParameters) {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  Serial.println("\n[WiFi] Connected to Wokwi-GUEST");

  client.setServer(mqtt_server, 1883);

  SensorData receivedData;

  for (;;) {
    if (!client.connected()) {
      if (client.connect("ESP32_RTOS_Client_CPS")) {
        Serial.println("[MQTT] Connected to Broker!");
      }
    }
    client.loop();

    // Ambil data dari Queue yang dikirim oleh DHT Task
    if (xQueueReceive(dhtQueue, &receivedData, portMAX_DELAY) == pdTRUE) {
      char payload[64];
      snprintf(payload, sizeof(payload), "{\"temp\":%.2f, \"humidity\":%.2f}", 
               receivedData.temperature, receivedData.humidity);

      if (client.connected()) {
        client.publish("cps/rtos/dht22", payload);
        Serial.print("[MQTT Sent]: ");
        Serial.println(payload);
      }
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

#endif
