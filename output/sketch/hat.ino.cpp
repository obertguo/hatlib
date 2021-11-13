#include <Arduino.h>
#line 1 "x:\\My Drive\\Apps\\Hat\\hat\\hat.ino"
#include "SDWrapper.h"
#include <SD.h>

SDWrapper sd;

#line 6 "x:\\My Drive\\Apps\\Hat\\hat\\hat.ino"
void setup();
#line 19 "x:\\My Drive\\Apps\\Hat\\hat\\hat.ino"
void loop();
#line 6 "x:\\My Drive\\Apps\\Hat\\hat\\hat.ino"
void setup(){
    Serial.begin(9600);
    while(!Serial){}
    sd = SDWrapper();
    String* s = sd.readRootDirectory();

    for(int i = 0; i < 5; ++i){
            if((s+i)->length() == 0) continue;
            Serial.println(*(s+i));
            Serial.println(i);
    }
}

void loop(){
    
}

