#include "SDWrapper.h"
#include <SD.h>

const int maxPatterns = 5;
const int numPixels = 64 * 8;
SDWrapper sd;

void setup(){
    Serial.begin(9600);
    while(!Serial){}
    sd = SDWrapper();

    //Allocate a 2d array for pixel data - (8 * 64) x 3. Think of a 1d array where each element is rgb data. 
    //This occupies ~1.5kB of SRAM space. On an Arduino UNO, the SRAM size is 2kB, and this will cause SRAM overflow. Best to use an Arduino MEGA instead or anything with more SRAM.
//    uint8_t** pixels PROGMEM = (uint8_t**) malloc(numPixels * sizeof(uint8_t*));
//
//    for(int row = 0; row < numPixels; ++row){
//        pixels[row] = (uint8_t*) malloc(3 * sizeof(uint8_t));
//    }
//    sd.readFile("file.hat", pixels);



      //Outputs .HAT files
      String* s = sd.readRootDirectory();
      for(int i = 0; i < maxPatterns; ++i){
         if((s+i)->length() == 0) break;
         Serial.println(*(s+i));
      }
}

void loop(){
    
}
