#include "8encoder.h"
#include "system.h"
#include "constants.h"
#include <math.h>

EncoderState::EncoderState() {
	m_photons.m_index = ENC_PHOTONS_CHNL;
	m_photons.m_red = ENC_PHOTONS_R_LED;
	m_photons.m_green = ENC_PHOTONS_G_LED;
	m_photons.m_blue = ENC_PHOTONS_B_LED;

	m_chroma.m_index = ENC_CHROMA_CHNL;
	m_chroma.m_red = ENC_CHROMA_R_LED;
	m_chroma.m_green = ENC_CHROMA_G_LED;
	m_chroma.m_blue = ENC_CHROMA_B_LED;

	m_mood.m_index = ENC_MOOD_CHNL;
	m_mood.m_red = ENC_MOOD_R_LED;
	m_mood.m_green = ENC_MOOD_G_LED;
	m_mood.m_blue = ENC_MOOD_B_LED;

	m_noiseCalibration.m_index = ENC_NOISE_CHNL;
	m_noiseCalibration.m_red = ENC_NOISE_R_LED;
	m_noiseCalibration.m_green = ENC_NOISE_G_LED;
	m_noiseCalibration.m_blue = ENC_NOISE_B_LED;

	m_mode.m_index = ENC_MODE_CHNL;
	m_mode.m_red = ENC_MODE_R_LED;
	m_mode.m_green = ENC_MODE_G_LED;
	m_mode.m_blue = ENC_MODE_B_LED;

	m_mirror.m_index = ENC_MIRROR_CHNL;
	m_mirror.m_red = ENC_MIRROR_R_LED;	
	m_mirror.m_green = ENC_MIRROR_G_LED;
	m_mirror.m_blue = ENC_MIRROR_B_LED;

	m_basecoat.m_index = ENC_BASECOAT_CHNL;
	m_basecoat.m_red = ENC_BASECOAT_R_LED;
	m_basecoat.m_green = ENC_BASECOAT_G_LED;
	m_basecoat.m_blue = ENC_BASECOAT_B_LED;


	for (uint8_t i = 0; i < 8; ++i) {
		m_channels[i] = nullptr;
	}

	m_channels[0] = (EncoderChannel*)&m_photons;
	m_channels[1] = (EncoderChannel*)&m_chroma;
	m_channels[2] = (EncoderChannel*)&m_mood;
	m_channels[3] = (EncoderChannel*)&m_noiseCalibration;
	m_channels[4] = (EncoderChannel*)&m_mode;
	m_channels[6] = (EncoderChannel*)&m_mirror;
	m_channels[7] = (EncoderChannel*)&m_basecoat;

}

//PUBLIC
Encoder8::Encoder8() {


}

Encoder8::~Encoder8() {
}

bool Encoder8::begin() {
	m_encoder8.begin();
	m_encoder8.resetAll();
	vTaskDelay(100);

	for (uint8_t i = 0; i < 8; ++i) {
		if (m_state.m_channels[i] != nullptr) {
			m_encoder8.writeRGB(m_state.m_channels[i]->m_index, m_state.m_channels[i]->m_red, m_state.m_channels[i]->m_green, m_state.m_channels[i]->m_blue);
			vTaskDelay(100);
		}
	}

	m_encoder8.end();

	return true;
}

void Encoder8::initState(float l_photons, float l_chroma, float l_mood) {
	m_state.m_photons.m_counter = (uint32_t)(l_photons + 0.5);
	m_state.m_chroma.m_counter = (uint32_t)(l_chroma + 0.5);
	m_state.m_mood.m_counter = (uint32_t)(l_mood + 0.5);
}

EncoderState* Encoder8::getState() {
	updateState();
	return &m_state;
}

bool Encoder8::getState(EncoderState& l_state) {
	updateState();

	l_state = m_state;

	return true;
}

//PRIVATE



bool Encoder8::updateState() {
	int32_t l_encoder;

	m_encoder8.begin();

	m_state.m_noiseCalibration.m_pressed = m_encoder8.getKeyPressed(m_state.m_noiseCalibration.m_index);
	m_state.m_mode.m_pressed = m_encoder8.getKeyPressed(m_state.m_mode.m_index);

	// New Functions for Mirror and Basecoat mode ON/OFF
    m_state.m_mirror.m_pressed = m_encoder8.getKeyPressed(m_state.m_mirror.m_index);
    m_state.m_basecoat.m_pressed = m_encoder8.getKeyPressed(m_state.m_basecoat.m_index);

    if (m_state.m_mirror.m_pressed) {
        m_state.m_mirror.m_fValue = 1;
    }

    if (m_state.m_basecoat.m_pressed) {
        m_state.m_basecoat.m_fValue = 1;
    }
	
	l_encoder = m_encoder8.getRelCounter(m_state.m_photons.m_index);
	if (l_encoder > 0) {
		m_state.m_photons.m_counter += pow(l_encoder, 6);
	}
	else {
		m_state.m_photons.m_counter -= pow(l_encoder, 6);;
	}

	if (m_state.m_photons.m_counter < 0) {
		m_state.m_photons.m_counter = 0;
	}
	else if (m_state.m_photons.m_counter > 8192) {
		m_state.m_photons.m_counter = 8192;
	}
	m_state.m_photons.m_fValue = m_state.m_photons.m_counter;


	l_encoder = m_encoder8.getRelCounter(m_state.m_chroma.m_index);
	if (l_encoder > 0) {
		m_state.m_chroma.m_counter += pow(l_encoder, 6);;
	}
	else {
		m_state.m_chroma.m_counter -= pow(l_encoder, 6);;
	}

	if (m_state.m_chroma.m_counter < 0) {
		m_state.m_chroma.m_counter = 0;
	}
	else if (m_state.m_chroma.m_counter > 8192) {
		m_state.m_chroma.m_counter = 8192;
	}
	m_state.m_chroma.m_fValue = m_state.m_chroma.m_counter;



	l_encoder = m_encoder8.getRelCounter(m_state.m_mood.m_index);
	if (l_encoder > 0) {
		m_state.m_mood.m_counter += pow(l_encoder, 6);;
	}
	else {
		m_state.m_mood.m_counter -= pow(l_encoder, 6);;
	}
	if (m_state.m_mood.m_counter < 0) {
		m_state.m_mood.m_counter = 0;
	}
	else if (m_state.m_mood.m_counter > 8192) {
		m_state.m_mood.m_counter = 8192;
	}
	m_state.m_mood.m_fValue = m_state.m_mood.m_counter;


	//m_encoder8.inputSwitch();

	m_encoder8.end();

	return true;
}