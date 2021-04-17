#include "Arduino.h"
#include "VMA11.h"


VMA11::VMA11(int resetPin, int sdioPin, int sclkPin) : Si4703_Breakout(resetPin,sdioPin,sclkPin)
{
  
}

int ReadRDSState = 0;
#define CLEARBUFFER 0
#define READUNTILEOL 1
#define READUNTILBEGIN 2
#define READUNTILEND 3
uint8_t position = 0;
char poststation[9];

char VMA11::readRDSRadioStation(char* rs)
{
	strcpy(rs,poststation);
}

char VMA11::readRDSRadioText(char* rt)
{	
	memset(poststation,0,9);
	char status = 0;
	switch(ReadRDSState)
	{
		case CLEARBUFFER:
		memset(rt,0,65);
		memset(radiotext,0,65);
		
			ReadRDSState++;
		break;
		case READUNTILEOL:
			if(strchr(rt,0x0D) == 0)//When no CR(Carriage Return) is found => read RDS
			{
				readRDS(poststation,rt,&position);
			}
			else
			{
				memset(rt,0,65);
				memset(radiotext,0,65);
				ReadRDSState++;
				position = 100;
			}
		break;
		case READUNTILBEGIN:			
			if(position)
			{
				readRDS(poststation,rt,&position);
			}
			else
			{
				ReadRDSState++;
				position = 100;
				memset(rt,0,65);
				memset(radiotext,0,65);
			}
		break;
		case READUNTILEND:
			if(strchr(rt,0x0D) == 0)
			{
				readRDS(poststation,rt,&position);
			}
			else
			{	
				if(CheckValidText(rt))
				{
					status = 1; //Text found
					ReadRDSState = 0;
				}		
			}
		break;
		default:
		break;
		
	}
	return status;
}

static inline void considerrdschar(char *buf, int place, char ch)
{	
		if (ch < 0x10 || ch > 0x7F)
		{
			if(ch != 0x0D)
				return;
		}
		buf[place] = ch;
}

//void Si4703_Breakout::readRDS(char* buffer, long timeout)
int VMA11::readRDS(char* ps, char* rt,uint8_t* pos)
{ 
	readRegisters();
	if(si4703_registers[STATUSRSSI] & (1<<RDSR)) {
		if (fakerds) {
			memset(rdsdata, 0, 8);
			rdschanged = 1;
		}
		
		fakerds = 0;
		
		//const uint16_t a = si4703_registers[RDSA];
		const uint16_t b = si4703_registers[RDSB];
		const uint16_t c = si4703_registers[RDSC];
		const uint16_t d = si4703_registers[RDSD];
		
		const uint8_t groupid = (b & 0xF000) >> 12;
		uint8_t version = b & 0x10;
		
		switch(groupid) {
			case RDS_PS: {
				const uint8_t index = (b & 0x3)*2;
				char Dh = (d & 0xFF00) >> 8;
				char Dl = d;
				
				considerrdschar(rdsdata, index, Dh);
				considerrdschar(rdsdata, index +1, Dl);
				
				rdschanged = 1;
			};
				break;
			case RDS_RT: {
				rdschanged = 1;
				uint8_t index = (b & 0xF)*4;
				*pos = index; 
				char Ch = (c & 0xFF00) >> 8;
				char Cl = c;
				char Dh = (d & 0xFF00) >> 8;
				char Dl = d;
				
				considerrdschar(radiotext, index, Ch);
				considerrdschar(radiotext, index +1, Cl);
				considerrdschar(radiotext, index +2, Dh);
				considerrdschar(radiotext, index +3, Dl);
			};
				break;
		}
	}
	
	const int change = rdschanged;
	if (change) {
		strcpy(ps, rdsdata);
		strcpy(rt, radiotext);
	}
	rdschanged = 0;
	return (change) ? ((fakerds) ? (RDS_FAKE) : (RDS_AVAILABLE)) : (RDS_NO);
}