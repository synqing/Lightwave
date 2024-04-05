#ifndef USER_FUNCTIONS_H
#define USER_FUNCTIONS_H

#include <stdint.h>
#include "settings.h"

void init();

void applicationTask();
void ledTask(void* arg);

void debugLog(const char* str, ...);
void debugLogLn(const char* str, ...);

void usbEventCallback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
#endif