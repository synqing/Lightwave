#include "application.h"

Application::Application() {

}

Application::~Application() {

}

void Application::init(LedChannel* l_pLedChannels, uint8_t l_channelsNum) {
	m_channelsNum = l_channelsNum;
	m_pLedChannels = l_pLedChannels;
}

void Application::task() {
	uint32_t t_now_us = micros();        // Timestamp for this loop, used by some core functions
	uint32_t t_now = t_now_us / 1000.0;  // Millisecond version

	function_id = 0;     // These are for debug_function_timing() in system.h to see what functions take up the most time
	check_knobs(t_now);  // (knobs.h)
	// Check if the knobs have changed

	function_id = 1;
	//check_buttons(t_now);  // (buttons.h)
	// Check if the buttons have changed

	function_id = 2;
	//check_settings(t_now);  // (system.h)
	// Check if the settings have changed

	function_id = 3;
	//check_serial(t_now);  // (serial_menu.h)
	// Check if UART commands are available

	function_id = 4;
	//run_p2p();  // (p2p.h)
	// Process P2P network packets to synchronize units

	function_id = 5;
	//acquire_sample_chunk(t_now);  // (i2s_audio.h)
	// Capture a frame of I2S audio (holy crap, FINALLY something about sound)

	function_id = 6;
	//run_sweet_spot();  // (led_utilities.h)
	// Based on the current audio volume, alter the Sweet Spot indicator LEDs

	// Calculates audio loudness (VU) using RMS, adjusting for noise floor based on calibration
	//calculate_vu();

	function_id = 7;
	//process_GDFT();  // (GDFT.h)
	// Execute GDFT and post-process
	// (If you're wondering about that weird acronym, check out the source file)

	// Watches the rate of change in the Goertzel bins to guide decisions for auto-color shifting
	//calculate_novelty(t_now);

	/*if (g_config.AUTO_COLOR_SHIFT == true) {  // Automatically cycle color based on density of positive spectral changes
		// Use the "novelty" findings of the above function to affect color shifting when auto-color shifts are enabled
		process_color_shift();
	}
	else {
		hue_position = 0;
		hue_shifting_mix = -0.35;
	}
	*/

	function_id = 8;
	//lookahead_smoothing();  // (GDFT.h)
	// Peek at upcoming frames to study/prevent flickering

	function_id = 8;
	//log_fps(t_now_us);  // (system.h)
	// Log the audio system FPS

	if (DEBUG_MODE == true) {
		function_id = 31;
		//debug_function_timing(t_now);
	}

	yield();  // Otherwise the ESP32 will collapse into a black hole or something
}


void Application::usbUpdateModeIndication() {
	uint8_t l_ledIndex = 0;
	uint8_t l_sweetIndex = 0;


	while (true) {
		for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
			leds_16[i] = { 0, 0, 0 };
		}

		if (msc_update_started == false) {
			leds_16[led_index] = { 0, 0, 0.25 };
			ledcWrite(SWEET_SPOT_LEFT_CHANNEL, sweet_order[sweet_index][0] * 512);
			ledcWrite(SWEET_SPOT_CENTER_CHANNEL, sweet_order[sweet_index][1] * 512);
			ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, sweet_order[sweet_index][2] * 512);
		}
		else {
			leds_16[NATIVE_RESOLUTION - 1 - led_index] = { 0, 0.25, 0 };
			ledcWrite(SWEET_SPOT_LEFT_CHANNEL, sweet_order[sweet_index][2] * 4095);
			ledcWrite(SWEET_SPOT_CENTER_CHANNEL, sweet_order[sweet_index][1] * 4095);
			ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, sweet_order[sweet_index][0] * 4095);
		}


		show_leds();

		if (led_index == 0 || led_index == NATIVE_RESOLUTION / 2) {
			sweet_index++;
			if (sweet_index >= 3) {
				sweet_index = 0;
			}
		}

		led_index++;
		if (led_index >= NATIVE_RESOLUTION) {
			led_index = 0;
		}
		yield();
	}


}
