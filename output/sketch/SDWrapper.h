#line 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\SDWrapper.h"
#ifndef SDWrapper_h
#define SDWrapper_h
#pragma once

#include "Arduino.h"
#include <FastLED.h>

/*
For reference: SPI pins for Arduino Mega. https://www.arduino.cc/en/reference/SPI
MISO = 50;
MOSI = 51;
SCK = 52;
CS = 53; 
*/

class SDWrapper{
    public:
        SDWrapper();
        
        String* readRootDirectory();
        void readFile(String filename, CRGB* pixels);
    private:
        bool initialize();
        bool initialized;
        bool isInitialized();
};

#endif
