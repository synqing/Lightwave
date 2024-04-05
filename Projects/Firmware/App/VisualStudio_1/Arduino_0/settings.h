#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

#define FIRMWARE_VERSION 40000  // Try "V" on the Serial port for this!
//                       MmmPP     M = Major version, m = Minor version, P = Patch version
//                                 (i.e 3.5.4 would be 30504)

#define DEBUG_MODE true

#define TERMINAL_MESSAGE_MAX_LENGTH 200

#define LED_CHNLS_NUM 3

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

#define ENC_MIRROR_CHNL    6      
#define ENC_MIRROR_R_LED   153 	//RGB LED channel pointig, RED channel
#define ENC_MIRROR_G_LED   204 	//RGB LED channel pointig, GREEN channel
#define ENC_MIRROR_B_LED   51 //RGB LED channel pointig, BLUE channel

#define ENC_BASECOAT_CHNL  7	      
#define ENC_BASECOAT_R_LED 153 	//RGB LED channel pointig, RED channel
#define ENC_BASECOAT_G_LED 51 	//RGB LED channel pointig, GREEN channel
#define ENC_BASECOAT_B_LED 204 	//RGB LED channel pointig, BLUE channel

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





#endif