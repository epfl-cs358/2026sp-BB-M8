#pragma once

void headRotation_init();         // configure stepper speed and create queue
void headRotation_begin();        // start FreeRTOS stepper task
void headRotation_step(int steps); // enqueue a step command (non-blocking)
