#line 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\test\\output\\sketch\\SDWrapper.h"
#line 1 "x:\\My Drive\\Apps\\Hat\\hat\\SDWrapper.h"
#ifndef SDWrapper_h
#define SDWrapper_h
#pragma once

#include "Arduino.h"


/*
SD pins 
MISO = 12;
MOSI = 11;
SCK = 13;
CS = 10; 
*/

class SDWrapper{
    public:
        SDWrapper();
        
        String* readRootDirectory();
        void readFile(String filename, uint8_t** pixels);
    private:
        bool initialize();
        bool initialized;
        bool isInitialized();
};

#endif
