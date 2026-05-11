#include "HeadRotation.hpp"
#include <Arduino.h>
#include <Stepper.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define IN1 12
#define IN2 13
#define IN3 14
#define IN4 15

static const int STEPS_PER_REV = 2048;

// IN1, IN3, IN2, IN4 order matches the 28BYJ-48 half-step sequence
static Stepper _stepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);
static QueueHandle_t _queue;

static void stepper_task(void*) {
    int steps;
    while (true) {
        if (xQueueReceive(_queue, &steps, portMAX_DELAY) == pdTRUE)
            _stepper.step(steps);
    }
}

void headRotation_init() {
    _stepper.setSpeed(10);
    _queue = xQueueCreate(4, sizeof(int));
}

void headRotation_begin() {
    xTaskCreate(stepper_task, "stepper", 3072, nullptr, 4, nullptr);
    Serial.println("[HeadRotation] stepper task started");
}

void headRotation_step(int steps) {
    xQueueSend(_queue, &steps, 0);  // non-blocking: drop if queue full
}
