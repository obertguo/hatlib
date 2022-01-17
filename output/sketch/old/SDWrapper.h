#line 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\old\\SDWrapper.h"
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
        void readFile(String filename);
    private:
        bool initialize();
        bool initialized;
        bool isInitialized();
};

#endif