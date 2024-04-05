#include "gpioExtender.h"
#include "system.h"

//PUBLIC

GpioExtender::GpioExtender(AW9523_Address l_address) {
	m_address = l_address;
}

GpioExtender::~GpioExtender() {

}

bool GpioExtender::begin() {
	if (m_inited) {
		return true;
	}

	debugLog("AW9523 GPIO Expander init");

	if (!m_aw9523b.begin()) {
		debugLog("AW9523 not found? Check wiring!");
		return false;
	}

	debugLog("AW9523 found!");
	m_inited = true;

	m_aw9523b.reset();
	m_aw9523b.setPort0Mode(AW_PUSH_PULL);

	/*
	m_aw9523.pinMode((uint8_t)ExtOutput::TOUCH_RST, OUTPUT);
	m_aw9523.pinMode((uint8_t)ExtOutput::BUS_OUT_EN, OUTPUT);
	m_aw9523.pinMode((uint8_t)ExtOutput::AW_RST, OUTPUT);
	m_aw9523.pinMode((uint8_t)ExtOutput::USB_OTG, OUTPUT);
	m_aw9523.pinMode((uint8_t)ExtOutput::CAM_RST, OUTPUT);
	m_aw9523.pinMode((uint8_t)ExtOutput::LCD_RST, OUTPUT);
	m_aw9523.pinMode((uint8_t)ExtOutput::BOOST_EN, OUTPUT);*/

	return true;
}

bool GpioExtender::setState(ExtOutput l_output, bool l_state) {
	begin();

	if ((uint8_t)l_output < 8) {
		if (l_state) {
			m_P0OutState |= (1 << (uint8_t)l_output);
		}
		else {
			m_P0OutState &= ~(1 << (uint8_t)l_output);
		}
		m_aw9523b.write(AW9523BPort::P0, m_P0OutState);
	}else {
		l_output = (ExtOutput) ((uint8_t) l_output - 8);
		if (l_state) {
			m_P1OutState |= (1 << (uint8_t)l_output);
		}
		else {
			m_P1OutState &= ~(1 << (uint8_t)l_output);
		}
		m_aw9523b.write(AW9523BPort::P1, m_P1OutState);
	}
	return true;
}

//PRIVATE

