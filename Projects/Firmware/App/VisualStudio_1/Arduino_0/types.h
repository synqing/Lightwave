#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <FastLED.h>
#include <FixedPoints.h>
#include <FixedPointsCommon.h>

struct ChannelConfig {
	// Synced values
	float   PHOTONS;
	float   CHROMA;
	float   MOOD;
	uint8_t LIGHTSHOW_MODE;
	bool    MIRROR_ENABLED;

	uint8_t  LED_TYPE;
	uint16_t LED_COUNT;
	uint16_t LED_COLOR_ORDER;
	bool     LED_INTERPOLATION;
	bool     REVERSE_ORDER;
};

struct MainConfig {
	ChannelConfig m_channels[LED_CHNLS_NUM];

	uint32_t SAMPLE_RATE;
	uint8_t  NOTE_OFFSET;
	uint8_t  SQUARE_ITER;
	uint16_t SAMPLES_PER_CHUNK;
	float    SENSITIVITY;
	bool     BOOT_ANIMATION;
	uint32_t SWEET_SPOT_MIN_LEVEL;
	uint32_t SWEET_SPOT_MAX_LEVEL;
	int32_t  DC_OFFSET;
	uint8_t  CHROMAGRAM_RANGE;
	bool     STANDBY_DIMMING;
	bool     IS_MAIN_UNIT;
	uint32_t MAX_CURRENT_MA;
	bool     TEMPORAL_DITHERING;
	bool     AUTO_COLOR_SHIFT;
	float    INCANDESCENT_FILTER;
	bool     INCANDESCENT_MODE;
	float    BULB_OPACITY;
	float    SATURATION;
	uint8_t  PRISM_COUNT;
	bool     BASE_COAT;
	float    VU_LEVEL_FLOOR;
};

enum reserved_dots {
	GRAPH_NEEDLE,
	GRAPH_DOT_1,
	GRAPH_DOT_2,
	GRAPH_DOT_3,
	GRAPH_DOT_4,
	GRAPH_DOT_5,
	RIPPLE_LEFT,
	RIPPLE_RIGHT,

	RESERVED_DOTS
};

enum knob_names {
	K_NONE,
	K_PHOTONS,
	K_CHROMA,
	K_MOOD
};

struct CRGB16 {  // Unsigned Q8.8 Fixed-point color channels
	SQ15x16 r;
	SQ15x16 g;
	SQ15x16 b;
};

struct DOT {
	SQ15x16 position;
	SQ15x16 last_position;
};

struct KNOB {
	SQ15x16  value;
	SQ15x16  last_value;
	SQ15x16  change_rate;
	uint32_t last_change;
};

enum led_types {
	LED_NEOPIXEL,
	LED_DOTSTAR
};



#endif