#include "SDWrapper.h"
#include <SD.h>

const int chipSelect = 10;
const int maxPatterns = 10;

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

void SDWrapper::readFile(String filename){
    if(!this->isInitialized()) return;
    
    //Quick file extension verification
    // if(!filename.endsWith(".HAT")) return;

    File patternFile = SD.open(filename);
    
    if(!patternFile) {
        Serial.println("Error opening file.");
        return;
    }

    uint8_t data[8 * 64] = {}; //0 fill
    int idx = 0;

    String currNum = "";
    uint8_t currentPixel = 

    //convert .hat values into 8x64 byte arraya 
    while(patternFile.available() && idx < sizeof(data)){
        char c = patternFile.read();

        if(c.toInt() == 0 && c != '0') {
          data[idx] = currNum.toInt(); 
          ++idx;
          currNum = "";
          continue;
        }

        currNum += c;
    }

    data[idx] = currNum.toInt(); //Don't forget to add the last number into the list after the loop ends

    for(auto el : data){
      Serial.println(el);
    }
      

    patternFile.close();
}

//Return a static reference to a String ref array of .hat filenames
String* SDWrapper::readRootDirectory(){
    static String patternFiles[maxPatterns]; 
    int idx = 0; //keeps track of curr idx when adding file name to array


    if(!this->isInitialized()) 
        return patternFiles;
    
    File root = SD.open("/");    
    
    //Continue finding hat files until max pattern is reached
    while(idx < maxPatterns){ 
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

    //Display hat files
    // for(int i = 0; i < maxPatterns; ++i){
    //         if(patternFiles[i].length() == 0) continue;
    //         Serial.println(patternFiles[i]);
    //         Serial.println(i);
    // }

    return patternFiles;
}

bool SDWrapper::initialize(){
    Serial.println("Initializing SD card...");

    if(!SD.begin(chipSelect)){
        Serial.println("Failed to initialize SD card. Things to check:");
        Serial.println("1. Is a card inserted? Is it in the right format? FAT16 or FAT32?");
        Serial.println("2. Double check your wiring. MISO = 12; MOSI = 11; SCK = 13; CS = 10; ");
        return false;
    }
    
    Serial.println("SD card initialized.");
    return true;
}
