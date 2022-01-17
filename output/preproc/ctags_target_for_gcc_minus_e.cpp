# 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\hat.ino"
# 2 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\hat.ino" 2
# 3 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\hat.ino" 2





SDWrapper* sd; //The SD wrapper likes to be initialized on the heap for some reason. Probably has something to do with the inner workings of the SD library? Not a huge deal I suppose. 
HatLib hatlib; //Store reference to hatlib object.
uint8_t numHatFiles = 0; //Keeps track of how many HAT files are on the SD card's root dir. 

String* hatFileNames; //Arduino string array of .HAT files on the SD card
CRGB pixels[512 /*Hat contains 64 * 8 pixels*/]; //Pixel buffer that stores RGB pixel information obtained from reading a .HAT file on the SD card.

void setup(){
    //Wait for serial monitor to load for debugging purposes.
    Serial.begin(9600);
    while(!Serial){}

    sd = new SDWrapper(); //Initialize SD library.
    hatFileNames = sd->readRootDirectory(); //Read hat files in root dir, the filenames are stored as an array of Arduino strings. 

    //Determine the number of hat files we've found by iterating through the filename array starting at the beginning. Once an empty string is encountered, we've found the number of HAT files.
    for(uint8_t i = 0; i < 6 /* The maximum pattern files we will load. Can be set to whatever positive integer you'd like of reasonable size before the Arduino encounters memory issues. There is also a corresponding max pattern constant in the SDWrapper.cpp file*/; ++i){
       if((hatFileNames+i)->length() == 0) break;
       ++numHatFiles;
       Serial.println(*(hatFileNames+i)); //Output .HAT file names on SD card for debugging purposes.
    }

    sd->readFile(hatFileNames[0], pixels); //Read data stored on the first hat file, the RGB data will be stored to the pixels array. (The pixels array is passed by reference)

    hatlib = HatLib(); //Initialize hat library
    hatlib.updatePixels(pixels); //Update hat's internal pixel reference.
}

PIXEL_SHIFT_DIRECTION currVerticalShift = UP_SHIFT;
PIXEL_SHIFT_DIRECTION currHorizontalShift = RIGHT_SHIFT;
PIXEL_SHIFT_DIRECTION currDir = currHorizontalShift;
uint8_t imgIdx = 0;

void loop(){
    delay(100 /* The number of microseconds per shift*/);
    hatlib.displayPixels(currDir);

    //Read change image button, if button is pressed, shift image index one over in a modulo fashion, and update pixel data.
    if(hatlib.readChangeImageButton()){
      imgIdx = (imgIdx + 1) % numHatFiles;
      sd->readFile(hatFileNames[imgIdx], pixels);
    }

    //Read button presses
    hatlib.readChangeShiftModeButton(currHorizontalShift, currVerticalShift, currDir);

    const char* fileName = hatFileNames[imgIdx].c_str(); //Current file name for the pattern displayed on the hat
    hatlib.displayMenu(fileName, currDir); //Display hat's menu
}
