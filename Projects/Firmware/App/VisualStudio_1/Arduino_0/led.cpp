#include "led.h"
#include "system.h"

void LedChannel::init(uint32_t l_ledsCount) {
	m_ledsCount = l_ledsCount;
	m_pLedsScaled = new CRGB16[l_ledsCount];
	m_pLedsOut = new CRGB[l_ledsCount];

	clear_all_led_buffers();
}

void LedChannel::scale_to_strip() {
	if (m_ledsCount == NATIVE_RESOLUTION) {
		// TODO: Potential cause of "Stuck Green LED"
		//
		// Why did I do a for() loop instead of memcpy?
		// Is this causing the green LED bug in unmirrored mode?
		//for (SQ15x16 i = 0; i < CONFIG.LED_COUNT; i++) {
		memcpy(m_pLedsScaled, leds_16, sizeof(CRGB16) * NATIVE_RESOLUTION);
		//leds_scaled[i.getInteger()] = leds_16[i.getInteger()];
	  //}
	}
	else {
		for (SQ15x16 i = 0; i < g_config.LED_COUNT; i++) {
			SQ15x16 prog = i / SQ15x16(g_config.LED_COUNT);

			// index is in the range of 0.0 - 128.0
			m_pLedsScaled[i.getInteger()] = lerp_led_16(prog * SQ15x16(128.0), leds_16);
		}
	}
}

void LedChannel::quantize_color(bool l_temporalDithering) {
	if (l_temporalDithering) {
		dither_step++;
		if (dither_step >= 4) {
			dither_step = 0;
		}

		static uint8_t noise_origin_r = 0;  // 0
		static uint8_t noise_origin_g = 0;  // 2
		static uint8_t noise_origin_b = 0;  // 4

		noise_origin_r += 1;
		noise_origin_g += 1;
		noise_origin_b += 1;

		for (uint16_t i = 0; i < m_ledsCount; i += 1) {
			// RED #####################################################
			SQ15x16 decimal_r = m_pLedsScaled[i].r * SQ15x16(254);
			SQ15x16 whole_r = decimal_r.getInteger();
			SQ15x16 fract_r = decimal_r - whole_r;

			if (fract_r >= dither_table[(noise_origin_r + i) % 4]) {
				whole_r += SQ15x16(1);
			}

			m_pLedsOut[i].r = whole_r.getInteger();

			// GREEN ###################################################
			SQ15x16 decimal_g = m_pLedsScaled[i].g * SQ15x16(254);
			SQ15x16 whole_g = decimal_g.getInteger();
			SQ15x16 fract_g = decimal_g - whole_g;

			if (fract_g >= dither_table[(noise_origin_g + i) % 4]) {
				whole_g += SQ15x16(1);
			}

			m_pLedsOut[i].g = whole_g.getInteger();

			// BLUE ####################################################
			SQ15x16 decimal_b = m_pLedsScaled[i].b * SQ15x16(254);
			SQ15x16 whole_b = decimal_b.getInteger();
			SQ15x16 fract_b = decimal_b - whole_b;

			if (fract_b >= dither_table[(noise_origin_b + i) % 4]) {
				whole_b += SQ15x16(1);
			}

			m_pLedsOut[i].b = whole_b.getInteger();
		}
	}
	else {
		for (uint16_t i = 0; i < m_ledsCount; i += 1) {
			m_pLedsOut[i].r = uint8_t(m_pLedsScaled[i].r * 255);
			m_pLedsOut[i].g = uint8_t(m_pLedsScaled[i].g * 255);
			m_pLedsOut[i].b = uint8_t(m_pLedsScaled[i].b * 255);
		}
	}
}

void LedChannel::reverse_leds() {
	uint16_t start = 0;
	uint16_t end = m_ledsCount - 1;
	while (start < end) {
		CRGB temp = m_pLedsOut[start];
		m_pLedsOut[start] = m_pLedsOut[end];
		m_pLedsOut[end] = temp;
		start++;
		end--;
	}
}

void LedChannel::clear_all_led_buffers() {
	for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
		leds_16[i] = { 0, 0, 0 };
		leds_16_temp[i] = { 0, 0, 0 };
		leds_16_fx[i] = { 0, 0, 0 };
	}

	if (m_pLedsScaled != nullptr) {
		for (uint16_t i = 0; i < m_ledsCount; i++) {
			m_pLedsScaled[i] = { 0, 0, 0 };
		}
	}

	if (m_pLedsOut != nullptr) {
		for (uint16_t i = 0; i < m_ledsCount; i++) {
			m_pLedsOut[i] = CRGB(0, 0, 0);
		}
	}
}

// Returns the linear interpolation of a floating point index in a CRGB array
// index is in the range of 0.0 - 128.0
CRGB16 LedChannel::lerp_led_16(SQ15x16 index, CRGB16* led_array) {
	int32_t index_whole = index.getInteger();
	SQ15x16 index_fract = index - (SQ15x16)index_whole;

	int32_t index_left = index_whole + 0;
	int32_t index_right = index_whole + 1;

	SQ15x16 mix_left = SQ15x16(1.0) - index_fract;
	SQ15x16 mix_right = SQ15x16(1.0) - mix_left;

	CRGB16 out_col;
	out_col.r = led_array[index_left].r * mix_left + led_array[index_right].r * mix_right;
	out_col.g = led_array[index_left].g * mix_left + led_array[index_right].g * mix_right;
	out_col.b = led_array[index_left].b * mix_left + led_array[index_right].b * mix_right;

	return out_col;
}

CRGB16 LedChannel::adjust_hue_and_saturation(CRGB16 color, SQ15x16 hue, SQ15x16 saturation) {
	// Store the RGB values
	SQ15x16 r = color.r, g = color.g, b = color.b;

	// Calculate maximum and minimum values of r, g, b
	SQ15x16 max_val = fmax_fixed(r, fmax_fixed(g, b));

	// Calculate the value of the HSV color
	SQ15x16 v = max_val;

	// Use the input saturation
	SQ15x16 s = saturation;

	// Prepare to convert HSV back to RGB
	SQ15x16 c = v * s;  // chroma
	SQ15x16 h_prime = fmod_fixed(hue * SQ15x16(6.0), SQ15x16(6.0));
	SQ15x16 x = c * (SQ15x16(1.0) - fabs_fixed(fmod_fixed(h_prime, SQ15x16(2.0)) - SQ15x16(1.0)));

	// Recalculate r, g, b based on the new hue and saturation
	if (h_prime >= 0 && h_prime < 1) {
		r = c;
		g = x;
		b = 0;
	}
	else if (h_prime >= 1 && h_prime < 2) {
		r = x;
		g = c;
		b = 0;
	}
	else if (h_prime >= 2 && h_prime < 3) {
		r = 0;
		g = c;
		b = x;
	}
	else if (h_prime >= 3 && h_prime < 4) {
		r = 0;
		g = x;
		b = c;
	}
	else if (h_prime >= 4 && h_prime < 5) {
		r = x;
		g = 0;
		b = c;
	}
	else if (h_prime >= 5 && h_prime < 6) {
		r = c;
		g = 0;
		b = x;
	}

	// Add the calculated difference to get the final RGB values
	SQ15x16 m = v - c;
	r += m;
	g += m;
	b += m;

	// Clamp the values between 0.0 and 1.0 to account for rounding errors
	r = fmax_fixed(SQ15x16(0.0), fmin_fixed(SQ15x16(1.0), r));
	g = fmax_fixed(SQ15x16(0.0), fmin_fixed(SQ15x16(1.0), g));
	b = fmax_fixed(SQ15x16(0.0), fmin_fixed(SQ15x16(1.0), b));

	// Return the resulting color
	CRGB16 result = { r, g, b };
	return result;
}

void LedChannel::render_noise_cal() {
	// Noise cal UI
	float noise_cal_progress = noise_iterations / 256.0;

	uint8_t prog_led_index = (NATIVE_RESOLUTION >> 1) * noise_cal_progress;
	float max_val = 0.0;
	for (uint16_t i = 0; i < NUM_FREQS; i++) {
		if (noise_samples[i] > max_val) {
			max_val = float(noise_samples[i]);
		}
	}
	for (uint16_t i = 0; i < NATIVE_RESOLUTION >> 1; i++) {
		if (i < prog_led_index) {
			float led_level = float(noise_samples[i]) / max_val;
			led_level = led_level * 0.9 + 0.1;
			leds_16_ui[64 + i] = hsv(0.859, g_config.SATURATION, led_level * led_level);
			leds_16_ui[63 - i] = leds_16_ui[64 + i];
		}
		else if (i == prog_led_index) {
			leds_16_ui[64 + i] = hsv(0.875, 1.0, 1.0);
			leds_16_ui[63 - i] = leds_16_ui[64 + i];

			ui_mask[64 + i] = 1.0;
			ui_mask[63 - i] = ui_mask[64 + i];

		}
		else {
			leds_16_ui[64 + i] = hsv(0, 0, 0);
			leds_16_ui[63 - i] = leds_16_ui[64 + i];
		}
	}

	if (noise_iterations > 192) {  // fade out towards end of calibration
		uint16_t iters_left = 64 - (noise_iterations - 192);
		float brightness_level = iters_left / 64.0;
		brightness_level *= brightness_level;
	}
}

void LedChannel::apply_brightness() {
	// This is only used to fade in when booting!
	if (millis() >= 1000 && noise_transition_queued == false && mode_transition_queued == false) {
		if (MASTER_BRIGHTNESS < 1.0) {
			MASTER_BRIGHTNESS += 0.005;
		}
		if (MASTER_BRIGHTNESS > 1.0) {
			MASTER_BRIGHTNESS = 1.00;
		}
	}

	SQ15x16 brightness = MASTER_BRIGHTNESS * (g_config.PHOTONS * g_config.PHOTONS) * silent_scale;

	for (uint16_t i = 0; i < 128; i++) {
		leds_16[i].r *= brightness;
		leds_16[i].g *= brightness;
		leds_16[i].b *= brightness;
	}

	clip_led_values();
}

void LedChannel::blocking_flash(CRGB16 col) {
	led_thread_halt = true;
	for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
		leds_16[i] = { 0, 0, 0 };
	}

	const uint8_t flash_times = 2;
	for (uint8_t f = 0; f < flash_times; f++) {
		for (uint8_t i = 0 + 48; i < NATIVE_RESOLUTION - 48; i++) {
			leds_16[i] = col;
		}
		show_leds();
		FastLED.delay(150);

		for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
			leds_16[i] = { 0, 0, 0 };
		}
		show_leds();
		FastLED.delay(150);
	}
	led_thread_halt = false;
}

void LedChannel::scale_image_to_half(CRGB16* led_array) {
	for (uint16_t i = 0; i < (NATIVE_RESOLUTION >> 1); i++) {
		leds_16_temp[i].r = led_array[i << 1].r * SQ15x16(0.5) + led_array[(i << 1) + 1].r * SQ15x16(0.5);
		leds_16_temp[i].g = led_array[i << 1].g * SQ15x16(0.5) + led_array[(i << 1) + 1].g * SQ15x16(0.5);
		leds_16_temp[i].b = led_array[i << 1].b * SQ15x16(0.5) + led_array[(i << 1) + 1].b * SQ15x16(0.5);
		leds_16_temp[64 + i] = { 0, 0, 0 };
	}

	memcpy(led_array, leds_16_temp, sizeof(CRGB16) * NATIVE_RESOLUTION);
}

void LedChannel::render_ui() {
	if (noise_complete == true) {
		if (current_knob == K_NONE) {
			// Close UI if open
			if (ui_mask_height > 0.005) {
				transition_ui_mask_to_height(0.0);
			}
		}
		else {
			if (current_knob == K_PHOTONS) {
				render_photons_graph();
			}
			else if (current_knob == K_CHROMA) {
				render_chroma_graph();
			}
			else if (current_knob == K_MOOD) {
				render_mood_graph();
			}

			// Open UI if closed
			transition_ui_mask_to_height(0.5);
		}
	}
	else {
		render_noise_cal();
	}

	if (ui_mask_height > 0.005 || noise_complete == false) {
		for (uint8_t i = 0; i < 128; i++) {
			SQ15x16 mix = ui_mask[i];
			SQ15x16 mix_inv = SQ15x16(1.0) - mix;

			if (mix > 0.0) {
				leds_16[i].r = leds_16[i].r * mix_inv + leds_16_ui[i].r * mix;
				leds_16[i].g = leds_16[i].g * mix_inv + leds_16_ui[i].g * mix;
				leds_16[i].b = leds_16[i].b * mix_inv + leds_16_ui[i].b * mix;
			}
		}
	}
}

void LedChannel::show_leds() {
	apply_brightness();

	// Tint the color image with an incandescent LUT to reduce harsh blues
	if (g_config.INCANDESCENT_FILTER > 0.0) {
		apply_incandescent_filter();
	}

	if (g_config.BASE_COAT == true) {
		if (g_config.PHOTONS <= 0.05) {
			base_coat_width_target = 0.0;
		}
		else {
			base_coat_width_target = 1.0;
		}

		SQ15x16 transition_speed = 0.05;
		if (base_coat_width < base_coat_width_target) {
			base_coat_width += (base_coat_width_target - base_coat_width) * transition_speed;
		}
		else if (base_coat_width > base_coat_width_target) {
			base_coat_width -= (base_coat_width - base_coat_width_target) * transition_speed;
		}

		SQ15x16 backdrop_divisor = 256.0;

		SQ15x16 bottom_value_r = 1 / backdrop_divisor;
		SQ15x16 bottom_value_g = 1 / backdrop_divisor;
		SQ15x16 bottom_value_b = 1 / backdrop_divisor;

		CRGB16 backdrop_color = { bottom_value_r, bottom_value_g, bottom_value_b };

		SQ15x16 base_coat_width_scaled = base_coat_width * silent_scale;

		if (base_coat_width_scaled > 0.01) {
			draw_line(leds_16, 0.5 - (base_coat_width_scaled * 0.5), 0.5 + (base_coat_width_scaled * 0.5), backdrop_color, 1.0);
		}

		/*
		for (uint8_t i = 0; i < 128; i++) {
		  if (leds_16[i].r < bottom_value_r) { leds_16[i].r = bottom_value_r; }
		  if (leds_16[i].g < bottom_value_g) { leds_16[i].g = bottom_value_g; }
		  if (leds_16[i].b < bottom_value_b) { leds_16[i].b = bottom_value_b; }
		}
		*/
	}

	render_ui();
	clip_led_values();

	for (uint8_t i = 0; i < LED_CHNLS_NUM; ++i) {
		g_ledChannel[i].scale_to_strip();

		g_ledChannel[i].quantize_color(g_config.TEMPORAL_DITHERING);

		if (g_config.REVERSE_ORDER == true) {
			g_ledChannel[i].reverse_leds();
		}
	}
}

void LedChannel::intro_animation() {
	const uint8_t particle_count = 16;
	particle particles[particle_count];

	m_masterBrightness = 1.0;
	ledcWrite(SWEET_SPOT_LEFT_CHANNEL, 0.0 * 4096);
	ledcWrite(SWEET_SPOT_CENTER_CHANNEL, 0.0 * 4096);
	ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, 0.0 * 4096);

	for (float progress = 0.3; progress <= 0.925; progress += 0.01) {
		float total_vals = 0.925 - 0.3;
		float brightness = (progress - 0.3) / total_vals;

		brightness *= brightness;

		m_masterBrightness = brightness;
		ledcWrite(SWEET_SPOT_LEFT_CHANNEL, brightness * 4096);
		ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, brightness * 4096);

		float pos = (cos(progress * 5) + 1) / 2.0;
		float pos_whole = pos * NATIVE_RESOLUTION;
		for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
			float delta = fabs(pos_whole - i);
			if (delta > 5.0) {
				delta = 5.0;
			}
			float led_level = 1.0 - (delta / 5.0);
			CRGB16 out_col = hsv(progress, 0, led_level);
			leds_16[i] = out_col;
		}
		show_leds();
	}

	for (uint8_t i = 0; i < LED_CHNLS_NUM; ++i) {
		g_ledChannel[i].clear_all_led_buffers();
	}

	for (uint8_t i = 0; i < particle_count; i++) {
		float prog = i / float(particle_count);
		particles[i].phase = 0.0;
		particles[i].speed = 0.002 * (i + 1);
		particles[i].col = hsv(prog, g_config.SATURATION, 1.0);
	}
	m_masterBrightness = 1.0;
	float center_brightness = 0.0;

	for (uint16_t i = 0; i < 50; i++) {
		if (center_brightness < 1.0) {
			center_brightness += 0.2;
			ledcWrite(SWEET_SPOT_CENTER_CHANNEL, (center_brightness * center_brightness) * 4096);
		}

		float dimming = 1.0;
		float anim_prog = i / 50.0;
		if (anim_prog >= 0.5) {
			anim_prog = (anim_prog - 0.5) * 2.0;
			dimming = 1.0 - anim_prog;
			dimming *= dimming;
			MASTER_BRIGHTNESS = dimming;
			ledcWrite(SWEET_SPOT_LEFT_CHANNEL, 0);
			ledcWrite(SWEET_SPOT_CENTER_CHANNEL, dimming * 4096);
			ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, 0);
		}
		else {
			anim_prog *= 2.0;
			dimming = 1.0 - anim_prog;
			dimming *= dimming;
			ledcWrite(SWEET_SPOT_LEFT_CHANNEL, dimming * 4096);
			ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, dimming * 4096);
		}

		for (uint8_t i = 0; i < LED_CHNLS_NUM; ++i) {
			g_ledChannel[i].clear_all_led_buffers();
		}

		for (uint8_t p = 0; p < particle_count; p++) {
			particles[p].phase += particles[p].speed;

			float pos = (sin(particles[p].phase * 5) + 1) / 2.0;
			float pos_whole = pos * NATIVE_RESOLUTION;
			for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
				float delta = fabs(pos_whole - i);
				if (delta > 10.0) {
					delta = 10.0;
				}
				float led_level = 1.0 - (delta / 10.0);
				led_level *= led_level;
				CRGB16 out_col = particles[p].col;
				out_col.r *= led_level;
				out_col.g *= led_level;
				out_col.b *= led_level;
				leds_16[i].r += out_col.r;
				leds_16[i].g += out_col.g;
				leds_16[i].b += out_col.b;
			}
		}
		show_leds();
		FastLED.delay(1);
	}
	m_masterBrightness = 0.0;


}

void LedChannel::shift_leds_up(CRGB16* led_array, uint16_t offset) {
	memcpy(leds_16_temp, led_array, sizeof(CRGB16) * NATIVE_RESOLUTION);
	memcpy(led_array + offset, leds_16_temp, (NATIVE_RESOLUTION - offset) * sizeof(CRGB16));
	memset(led_array, 0, offset * sizeof(CRGB16));
}

void LedChannel::shift_leds_down(CRGB* led_array, uint16_t offset) {
	memcpy(led_array, led_array + offset, (NATIVE_RESOLUTION - offset) * sizeof(CRGB));
	memset(led_array + (NATIVE_RESOLUTION - offset), 0, offset * sizeof(CRGB));
}

void LedChannel::mirror_image_downwards(CRGB16* led_array) {
	for (uint8_t i = 0; i < (NATIVE_RESOLUTION >> 1); i++) {
		leds_16_temp[64 + i] = led_array[64 + i];
		leds_16_temp[63 - i] = led_array[64 + i];
	}

	memcpy(led_array, leds_16_temp, sizeof(CRGB16) * NATIVE_RESOLUTION);
}

void LedChannel::unmirror() {
	for (uint16_t i = 0; i < NATIVE_RESOLUTION; i++) {  // Interpolation
		SQ15x16 index = 64 + (i / 2.0);

		int32_t index_whole = index.getInteger();
		SQ15x16 index_fract = index - (SQ15x16)index_whole;

		int32_t index_left = index_whole + 0;
		int32_t index_right = index_whole + 1;

		SQ15x16 mix_left = SQ15x16(1.0) - index_fract;
		SQ15x16 mix_right = SQ15x16(1.0) - mix_left;

		CRGB16 out_col;
		out_col.r = leds_16[index_left].r * mix_left + leds_16[index_right].r * mix_right;
		out_col.g = leds_16[index_left].g * mix_left + leds_16[index_right].g * mix_right;
		out_col.b = leds_16[index_left].b * mix_left + leds_16[index_right].b * mix_right;

		leds_16_temp[i] = out_col;
	}

	memcpy(leds_16, leds_16_temp, sizeof(CRGB16) * NATIVE_RESOLUTION);
}









































CRGB16* LedChannel::getScalled() {
	if (m_pLedsScaled == nullptr) {
		while (1) {
			debugLog("LedChannel::getScalled nullptr");
			vTaskDelay(100);
		}
	}
	else {
		return m_pLedsScaled;
	}
}

CRGB* LedChannel::getLedsOut() {
	if (m_pLedsOut == nullptr) {
		while (1) {
			debugLog("LedChannel::getScalled nullptr");
			vTaskDelay(100);
		}
	}
	else {
		return m_pLedsOut;
	}
}

uint16_t LedChannel::getLedsCount() {
	return m_ledsCount;
}







void LedChannel::usbUpdateAnimation(bool l_mscUpdateStarted) {
	const uint8_t l_sweetOrder[3][3] = {
				{1, 0, 0},
  {0, 1, 0},
  {0, 0, 1}
	};


	m_masterBrightness = 1.0;



	for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
		leds_16[i] = { 0, 0, 0 };
	}

	if (l_mscUpdateStarted == false) {
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




}

LedChannel g_ledChannel[LED_CHNLS_NUM];

