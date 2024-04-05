#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <FastLED.h>
#include "settings.h"
#include "types.h"

class LedChannel {
public:
	void init(uint32_t l_ledsCount);
	void scale_to_strip();
	void quantize_color(bool l_temporalDithering);
	void reverse_leds();
	void clear_all_led_buffers();
	CRGB16 lerp_led_16(SQ15x16 index, CRGB16* led_array);
	CRGB16 adjust_hue_and_saturation(CRGB16 color, SQ15x16 hue, SQ15x16 saturation);
	void render_noise_cal();
	void apply_brightness();
	void blocking_flash(CRGB16 col);
	void scale_image_to_half(CRGB16* led_array);
	void render_ui();
	void show_leds();
	void intro_animation();
	void shift_leds_up(CRGB16* led_array, uint16_t offset);
	void shift_leds_down(CRGB* led_array, uint16_t offset);
	void mirror_image_downwards(CRGB16* led_array);
	void unmirror();



	CRGB16* getScalled();
	CRGB* getLedsOut();
	uint16_t getLedsCount();






	void usbUpdateAnimation(bool l_mscUpdateStarted);


private:
	//Display buffers
	CRGB16  leds_16[128];
	CRGB16  leds_16_prev[128];
	CRGB16  leds_16_fx[128];
	CRGB16  leds_16_fx_2[128];
	CRGB16  leds_16_temp[128];
	CRGB16  leds_16_ui[128];



	uint16_t m_ledsCount;
	CRGB16* m_pLedsScaled = nullptr;
	CRGB* m_pLedsOut = nullptr;

	//WIP BELOW
	float m_masterBrightness = 0.0;
	float m_lastSample = 0;
};

extern LedChannel g_ledChannel[];

#endif