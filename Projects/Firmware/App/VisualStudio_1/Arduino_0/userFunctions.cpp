#include "userFunctions.h"
#include <Arduino.h>
#include <WiFi.h>         // Needed for Station Mode
#include <esp_now.h>      // P2P wireless communication library (p2p.h below)
#include <esp_random.h>   // RNG Functions
#include <FastLED.h>      // Handles LED color data and display
#include <FS.h>           // Filesystem functions (bridge_fs.h below)
#include <LittleFS.h>     // LittleFS implementation
#include <Ticker.h>       // Scheduled tasks library
#include <USB.h>          // USB Connection handling
#include <FirmwareMSC.h>  // Allows firmware updates via USB MSC
#include <FixedPoints.h>
#include <FixedPointsCommon.h>

#include "application.h"
#include "gpioExtender.h"
#include "8encoder.h"

USBCDC g_USBSerial;

FirmwareMSC g_MSCUpdate;
bool g_mscUpdateFlag = false;

GpioExtender g_gpioExtender;
Encoder8 g_encoder;

Application p_application;

void init() {


}

void applicationTask() {

}

void ledTask(void* arg) {
	while (true) {
		/*if (led_thread_halt == false) {                                             // If we're not gathering ambient noise data
			if (mode_transition_queued == true || noise_transition_queued == true) {  // If transition queued
				run_transition_fade();                                                  // (led_utilities.h) Fade to black between modes
			}

			get_smooth_spectrogram();
			make_smooth_chromagram();

			// Based on the value of CONFIG.LIGHTSHOW_MODE, we call a
			// different rendering function from lightshow_modes.h:

			if (g_config.LIGHTSHOW_MODE == LIGHT_MODE_GDFT) {
				light_mode_gdft();  // (lightshow_modes.h) GDFT spectrogram display
			}
			else if (g_config.LIGHTSHOW_MODE == LIGHT_MODE_GDFT_CHROMAGRAM) {
				light_mode_chromagram_gradient();  //light_mode_chromagram_gradient();  // (lightshow_modes.h) GDFT chromagram display
			}
			else if (g_config.LIGHTSHOW_MODE == LIGHT_MODE_GDFT_CHROMAGRAM_DOTS) {
				light_mode_chromagram_dots();  //light_mode_chromagram_dots();  // (lightshow_modes.h) GDFT chromagram display
			}
			else if (g_config.LIGHTSHOW_MODE == LIGHT_MODE_BLOOM) {
				light_mode_bloom();  // (lightshow_modes.h) Bloom Mode display
			}
			else if (g_config.LIGHTSHOW_MODE == LIGHT_MODE_VU_DOT) {
				light_mode_vu_dot();  // (lightshow_modes.h) VU Mode display (dot version)
			}
			else if (g_config.LIGHTSHOW_MODE == LIGHT_MODE_KALEIDOSCOPE) {
				light_mode_kaleidoscope();  // (lightshow_modes.h) Kaleidoscope Mode display
			}

			if (g_config.PRISM_COUNT > 0) {
				apply_prism_effect(g_config.PRISM_COUNT, 0.25);
			}

			// Render bulb filter
			if (g_config.BULB_OPACITY > 0.00) {
				render_bulb_cover();
			}

			// If forcing monochromatic incandescent output
			for (uint8_t uint8_t i = 0; i < )
				if (g_config.INCANDESCENT_MODE == true) {
					for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
						leds_16[i] = adjust_hue_and_saturation(leds_16[i], 0.05, 0.95);
					}
				}

			if (g_config.MIRROR_ENABLED == false) {  // Mirroring logic
				unmirror();
			}

			show_leds();  // This sends final RGB data to the LEDS (led_utilities.h)
			LED_FPS = FastLED.getFPS();
		}
		else {
			vTaskDelay(0);
		}

		if (g_config.LED_TYPE == LED_NEOPIXEL) {
			//vTaskDelay(1); // delay for 1ms to avoid hogging the CPU
		}
		else if (g_config.LED_TYPE == LED_DOTSTAR) {  // More delay to compensate for faster LEDs
			//vTaskDelay(3);                              // delay for 3ms to avoid hogging the CPU
		}*/
	}
}




void enable_usb_update_mode() {
	USB.onEvent(usbEventCallback);

	g_MSCUpdate.onEvent(usbEventCallback);
	g_MSCUpdate.begin();
}

void debugLog(const char* l_str, ...)
{
	va_list l_arg;
	va_start(l_arg, l_str);
	g_USBSerial.printf(l_str, l_arg);	
	va_end(l_arg);
}

void debugLogLn(const char* l_str, ...)
{
	va_list l_arg;
	va_start(l_arg, l_str);
	g_USBSerial.printf(l_str, l_arg);
	va_end(l_arg);

	g_USBSerial.print("\r\n");
}

void usbEventCallback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	if (event_base == ARDUINO_USB_EVENTS) {
		//arduino_usb_event_data_t * data = (arduino_usb_event_data_t*)event_data;
		switch (event_id) {
		case ARDUINO_USB_STARTED_EVENT:
			//Serial0.println("USB PLUGGED");
			break;
		case ARDUINO_USB_STOPPED_EVENT:
			//Serial0.println("USB UNPLUGGED");
			break;
		case ARDUINO_USB_SUSPEND_EVENT:
			//Serial0.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en);
			break;
		case ARDUINO_USB_RESUME_EVENT:
			//Serial0.println("USB RESUMED");
			break;

		default:
			break;
		}
	}
	else if (event_base == ARDUINO_FIRMWARE_MSC_EVENTS) {
		//arduino_firmware_msc_event_data_t * data = (arduino_firmware_msc_event_data_t*)event_data;
		switch (event_id) {
		case ARDUINO_FIRMWARE_MSC_START_EVENT:
			//Serial0.println("MSC Update Start");
			g_mscUpdateFlag = true;
			break;
		case ARDUINO_FIRMWARE_MSC_WRITE_EVENT:
			//HWSerial.printf("MSC Update Write %u bytes at offset %u\n", data->write.size, data->write.offset);
			//Serial0.print(".");
			break;
		case ARDUINO_FIRMWARE_MSC_END_EVENT:
			//Serial0.printf("\nMSC Update End: %u bytes\n", data->end.size);
			break;
		case ARDUINO_FIRMWARE_MSC_ERROR_EVENT:
			//Serial0.printf("MSC Update ERROR! Progress: %u bytes\n", data->error.size);
			break;
		case ARDUINO_FIRMWARE_MSC_POWER_EVENT:
			//Serial0.printf("MSC Update Power: power: %u, start: %u, eject: %u", data->power.power_condition, data->power.start, data->power.load_eject);
			break;

		default:
			break;
		}
	}
}