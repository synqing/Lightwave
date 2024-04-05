#include "userFunctions.h"

TaskHandle_t led_task;

// Setup, runs only one time ---------------------------------------------------------
void setup() {
  init();  // (system.h) Initialize all hardware and arrays
  // Create thread specifically for LED updates
  xTaskCreatePinnedToCore(ledTask, "ledTask", 8192, NULL, tskIDLE_PRIORITY + 1, &led_task, 1);
}

// Loop, runs forever after setup() --------------------------------------------------
void loop() {
    applicationTask();
}