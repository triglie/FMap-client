#include <VMA11.h>
#include <Wire.h>

int resetPin = 52;
int SDIO = 20;
int SCLK = 21;

VMA11 radio(resetPin, SDIO, SCLK);
int channel;
int volume;
char rdsname[9];
char rdsrt[65];
char previousRadioText[65];
uint8_t lastChar;
void setup()
{
  Serial.begin(9600);
  Serial.println("\n\nVMA11 Test Sketch");
  Serial.println("===========================");  
  Serial.println("a b     Favourite stations");
  Serial.println("+ -     Volume (max 15)");
  Serial.println("u d     Seek up / down");
  Serial.println("r       Listen for RDS Data (15 sec timeout)");
  Serial.println("R       Listen for RDS Data (15 sec timeout)");
  Serial.println("Send me a command letter.");
  

  radio.powerOn();
  radio.setVolume(1);
  volume=1;
  radio.setChannel(1021);

  memset(previousRadioText,0,65);
  memset(rdsrt,0,65);
}

void loop()
{
  if(radio.readRDSRadioText(rdsrt))
  {
    
    if(strcmp(rdsrt,previousRadioText))
    {
      Serial.println(rdsrt);          
      strcpy(previousRadioText,rdsrt);
    }
    
  }  
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'u') 
    {
      channel = radio.seekUp();
      displayInfo();
    } 
    else if (ch == 'd') 
    {
      channel = radio.seekDown();
      displayInfo();
    } 
    else if (ch == '+') 
    {
      volume ++;
      if (volume >=16) volume = 15;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == '-') 
    {
      volume --;
      if (volume < 0) volume = 0;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == 'a')
    {
      channel = 1015; // Rock FM
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'b')
    {
      channel = 925; // KBCO in Boulder
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'r')
    {
      // The calling of readRDS and printing of rdsname really need
      // to be looped to catch all of the data...
      // this will just print a snapshot of what is in the Si4703 RDS buffer...
      radio.readRDSRadioText(rdsname);
      Serial.println(rdsname);
    }
    else if (ch == 'R')
    {
      // The calling of readRDS and printing of rdsrt really need
      // to be looped to catch all of the data...
      // this will just print a snapshot of what is in the Si4703 RDS buffer...
      //Serial.println("RDS listening - screen");
      
      radio.readRDS(rdsname,rdsrt,&lastChar);
      Serial.println(rdsrt);         
    }
  }
}

void displayInfo()
{
   Serial.print("Channel:"); Serial.print(channel); 
   Serial.print(" Volume:"); Serial.println(volume); 
}
