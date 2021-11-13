#include "SDWrapper.h"
#include <SD.h>

const int maxPatterns = 5;
SDWrapper sd;

void setup(){
    Serial.begin(9600);
    while(!Serial){}
    sd = SDWrapper();
//    String* s = sd.readRootDirectory();
//
//     for(int i = 0; i < maxPatterns; ++i){
//         if((s+i)->length() == 0) continue;
//         Serial.println(*(s+i));
//         Serial.println(i);
//     }

    sd.readFile("file.hat");
}

void loop(){
    
}
