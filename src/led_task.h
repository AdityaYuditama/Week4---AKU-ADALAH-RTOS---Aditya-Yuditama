#ifndef LED_TASK_H
#define LED_TASK_H

#include <Arduino.h>

#define LED_RED_PIN 4
#define LED_GREEN_PIN 2
#define BUTTON_PIN 14

extern SemaphoreHandle_t buttonSemaphore;

// Interrupt Service Routine (ISR) - Dijalankan saat tombol ditekan
void IRAM_ATTR buttonISR() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(buttonSemaphore, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void TaskControlLED(void *pvParameters) {
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Pasang Interrupt pada tombol (Rising edge / saat ditekan)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);

  bool ledState = false;
  digitalWrite(LED_GREEN_PIN, HIGH); // Status normal: LED Hijau menyala

  for (;;) {
    // Menunggu sinyal interrupt dari tombol via Semaphore
    if (xSemaphoreTake(buttonSemaphore, portMAX_DELAY) == pdTRUE) {
      ledState = !ledState;
      digitalWrite(LED_RED_PIN, ledState ? HIGH : LOW);
      digitalWrite(LED_GREEN_PIN, ledState ? LOW : HIGH);
      Serial.println("[ISR] Button Pressed! Toggled LED Status.");
    }
  }
}

#endif