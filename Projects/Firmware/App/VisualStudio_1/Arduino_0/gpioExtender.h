#ifndef GPIO_EXTENDER_H
#define GPIO_EXTENDER_H

#include <stdint.h>
#include "AW9523B.h"

enum class AW9523_Address {
	_00 = 0x58,
	_01 = 0x59,
	_10 = 0x5A,
	_11 = 0x5B
};

enum class ExtOutput {
	TOUCH_RST = AW9523B_PIN0,
	BUS_OUT_EN = AW9523B_PIN1,
	AW_RST = AW9523B_PIN2,
	USB_OTG = AW9523B_PIN5,
	CAM_RST = AW9523B_PIN8,
	LCD_RST = AW9523B_PIN9,
	BOOST_EN = AW9523B_PIN15
};

class GpioExtender {
public:
	GpioExtender(AW9523_Address l_address = AW9523_Address::_00);
	~GpioExtender();
	bool begin();

	bool setState(ExtOutput l_output, bool l_state);
private:
	bool m_inited = false;
	AW9523_Address m_address;
	AW9523B m_aw9523b;

	uint8_t m_P0OutState = 0;
	uint8_t m_P1OutState = 0;
};


#endif