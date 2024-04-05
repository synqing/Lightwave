#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdint.h>
#include "settings.h"
#include "led.h"

class Application {
public:
	Application();
	~Application();

	void init(LedChannel* l_pLedChannels, uint8_t l_channelsNum);
	void task();

	void usbUpdateModeIndication();




private:
	MainConfig m_config;
	MainConfig m_configDefault; // Used for resetting to default values at runtime
	
	LedChannel *m_pLedChannels = nullptr;
	uint8_t m_channelsNum = 0;



};



#endif