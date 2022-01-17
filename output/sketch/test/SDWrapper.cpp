#line 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\test\\SDWrapper.cpp"
#include "SDWrapper.h"
#include <SD.h>

#define CHIPSELECT 10
#define MAXPATTERNS 5
#define NUMPIXELS 512 //8 x 64

SDWrapper::SDWrapper(){
    this->initialized = this->initialize();
}

bool SDWrapper::isInitialized(){
    if(!this->initialized){
        Serial.println("Please initialize the SD card first!");
        return false;
    }
    return true;
}

void SDWrapper::readFile(String filename, uint8_t** pixels){
    if(!this->isInitialized()) return;
    
    //Quick file extension verification
    if(!filename.endsWith(".HAT")) return;

    File patternFile = SD.open(filename);

    if(!patternFile) {
        Serial.println("Error opening file.");
        return;
    }

    int idx = 0;
    int rgbIdx = 0;
    String currNum = "";

    //convert .hat values into (64 * 8) x 3 byte array - a 2D array where each element is an array of length 3 of rgb values for a pixel
    while(patternFile.available() && idx < NUMPIXELS){
        char c = (char)patternFile.read();
        if(c == ',' || c == '\n'){
            pixels[idx][rgbIdx] = currNum.toInt(); 

            if(c == '\n') {
              ++idx;
            }
            
            rgbIdx = (rgbIdx + 1) % 3;
            currNum = "";
        }
        else{
            currNum += c;
        }
      }
      patternFile.close();
    }

//Return a static reference to a String ref array of .hat filenames
String* SDWrapper::readRootDirectory(){
    static String patternFiles[MAXPATTERNS]; 
    int idx = 0; //keeps track of curr idx when adding file name to array


    if(!this->isInitialized()) 
        return patternFiles;
    
    File root = SD.open("/");    
    
    //Continue finding hat files until max pattern is reached
    while(idx < MAXPATTERNS){ 
        File entry = root.openNextFile();
        if(!entry) break;

        if(!entry.isDirectory()){
            String file = entry.name();
            
            if(file.endsWith(".HAT")){
                patternFiles[idx] = file;
                ++idx;
            }
        }
        entry.close();
    }
    return patternFiles;
}

bool SDWrapper::initialize(){
    Serial.println("Initializing SD card...");

    if(!SD.begin(CHIPSELECT)){
        Serial.println("Failed to initialize SD card. Things to check:");
        Serial.println("1. Is a card inserted? Is it in the right format? FAT16 or FAT32?");
        Serial.println("2. Double check your wiring. MISO = 12; MOSI = 11; SCK = 13; CS = 10; ");
        return false;
    }
    
    Serial.println("SD card initialized.");
    return true;
}
