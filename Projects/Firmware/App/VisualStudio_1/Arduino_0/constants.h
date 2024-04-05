#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>
#include <FastLED.h>
#include <FixedPoints.h>
#include <FixedPointsCommon.h>
#include <driver/i2s.h>

// AUDIO #######################################################

#define SERIAL_BAUD 230400
#define DEFAULT_SAMPLE_RATE 12800
#define SAMPLE_HISTORY_LENGTH 4096

// Don't change this unless you're willing to do a lot of other work on the code :/
#define NATIVE_RESOLUTION 128
#define NUM_FREQS 64
#define NUM_ZONES 2

#define SPECTRAL_HISTORY_LENGTH 5

#define MAX_DOTS 128

// GPIO PINS #######################################################
#define LED_DATA_PIN    G9 //LED, OUT
#define LED_CLOCK_PIN   G6 //LED, OUT

/*BEGIN 8ENCODER SECTION*/
#define ENCODER_SDA_PIN G2
#define ENCODER_SCL_PIN G1

#define ENC_PHOTONS_CHNL   0 //Encoder channel for PHOTONS setup
#define ENC_PHOTONS_R_LED  100 //RGB LED channel pointig, RED channel
#define ENC_PHOTONS_G_LED  0 //RGB LED channel pointig, GREEN channel
#define ENC_PHOTONS_B_LED  0 //RGB LED channel pointig, BLUE channel

#define ENC_CHROMA_CHNL    1
#define ENC_CHROMA_R_LED   0 //RGB LED channel pointig, RED channel
#define ENC_CHROMA_G_LED   100 //RGB LED channel pointig, GREEN channel
#define ENC_CHROMA_B_LED   0 //RGB LED channel pointig, BLUE channel

#define ENC_MOOD_CHNL      2 
#define ENC_MOOD_R_LED     0 //RGB LED channel pointig, RED channel
#define ENC_MOOD_G_LED     0 //RGB LED channel pointig, GREEN channel
#define ENC_MOOD_B_LED     100 //RGB LED channel pointig, BLUE channel

#define ENC_NOISE_CHNL     3
#define ENC_NOISE_R_LED    100 //RGB LED channel pointig, RED channel
#define ENC_NOISE_G_LED    100 //RGB LED channel pointig, GREEN channel
#define ENC_NOISE_B_LED    0 //RGB LED channel pointig, BLUE channel

#define ENC_MODE_CHNL      4      
#define ENC_MODE_R_LED     0 //RGB LED channel pointig, RED channel
#define ENC_MODE_G_LED     100 //RGB LED channel pointig, GREEN channel
#define ENC_MODE_B_LED     100 //RGB LED channel pointig, BLUE channel

/*END 8ENCODER SECTION*/

//#define RNG_SEED_PIN    

#define SWEET_SPOT_LEFT_PIN    -1//Digital, OUT. Indicates about music loudness
#define SWEET_SPOT_CENTER_PIN  -1//Digital, OUT. Indicates about music loudness
#define SWEET_SPOT_RIGHT_PIN   -1//Digital, OUT. Indicates about music loudness

#define SWEET_SPOT_LEFT_CHANNEL   -1//Digital, WS2812. Indicates about music loudness
#define SWEET_SPOT_CENTER_CHANNEL -1//Digital, WS2812. Indicates about music loudness
#define SWEET_SPOT_RIGHT_CHANNEL  -1//Digital, WS2812. Indicates about music loudness

// OTHER #######################################################
#define TWOPI 6.28318530
#define FOURPI 12.56637061
#define SIXPI 18.84955593

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

extern const float notes[];
extern const SQ15x16 dither_table[];
extern const SQ15x16 note_colors[];
extern const SQ15x16 hue_lookup[][3];
extern const CRGB16 incandescent_lookup;

#endif