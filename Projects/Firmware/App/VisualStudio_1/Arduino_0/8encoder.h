#ifndef _8ENCODER_H
#define _8ENCODER_H

#include <stdint.h>
#include "m5rotate8.h"

struct EncoderChannel {
	EncoderChannel(uint8_t l_channel = 0, uint8_t l_red = 0, uint8_t l_green = 0, uint8_t l_blue = 0) {
		m_index = l_channel;
		m_red = l_red;
		m_green = l_green;
		m_blue = l_blue;
	}
	uint8_t m_index = 0; //Channel index	
	
	bool m_pressed = false;
	int32_t m_counter = 0;
	//int16_t m_value;

	uint8_t m_red = 0;
	uint8_t m_green = 0;
	uint8_t m_blue = 0;
};

struct Button : public EncoderChannel {
	bool m_pressedFlag = false;
	uint32_t last_down = 0;
	uint32_t last_up = 0;
	Button() {

	}
};

struct Knob : public EncoderChannel {
public:
	float m_fValue = 0.00;
	Knob() {

	}
};

struct EncoderState {
	Knob m_photons;
	Knob m_chroma;
	Knob m_mood;
	Button m_noiseCalibration;
	Button m_mode;
	Button m_mirror
	Button m_basecoat

	EncoderChannel* m_channels[8];

	EncoderState();
};

class Encoder8 {
public:
	Encoder8();
	~Encoder8();

	bool begin();

	void initState(float l_photons, float l_chroma, float l_mood);
	EncoderState* getState();
	bool getState(EncoderState& l_state);
private:
	EncoderState m_state;
	M5ROTATE8 m_encoder8;

	bool updateState();
};


#endif