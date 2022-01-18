#include "SDWrapper.h"
#include "HatConstants.h"
#include <SD.h>

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

void SDWrapper::readFile(String filename, CRGB* pixels){
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

    //Extract RGB data from hat file, and store the values in the pixels array.
    while(patternFile.available() && idx < NUM_PIXELS){
        char c = (char)patternFile.read();
        if(c == ',' || c == '\n'){
            pixels[idx][rgbIdx] = currNum.toInt(); 
            rgbIdx = (rgbIdx + 1) % 3;
            currNum = "";
            
            if(c == '\n') ++idx;
        }
        else currNum += c;
      }
      patternFile.close();
    }

//Return a static reference to a String ref array of .hat filenames
String* SDWrapper::readRootDirectory(){
    static String patternFiles[MAX_PATTERNS]; 
    int idx = 0; //keeps track of curr idx when adding file name to array

    if(!this->isInitialized()) 
        return patternFiles;
    
    File root = SD.open("/");    
    
    //Continue finding hat files until max pattern is reached
    while(idx < MAX_PATTERNS){ 
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

    if(!SD.begin(CHIP_SELECT_PIN)){
        Serial.println("Failed to initialize SD card. Things to check:");
        Serial.println("1. Is a card inserted? Is it in the right format? FAT16 or FAT32?");
        Serial.println("2. Double check your wiring. MISO = 50; MOSI = 51; SCK = 52; CS = 53; ");
        return false;
    }
    
    Serial.println("SD card initialized.");
    return true;
}