#ifndef __VMA11_H__
#define __VMA11_H__
#endif
#include "Si4703_Breakout_Modified.h"
#include "Arduino.h"
class VMA11 : public Si4703_Breakout
{
	public:
		VMA11(int resetPin, int sdioPin, int sclkPin);
		int readRDS(char* ps, char* rt,uint8_t* pos);
		char readRDSRadioText(char* rt);
		char readRDSRadioStation(char* rs);
};