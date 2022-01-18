#include <FastLED.h>
#include "SDWrapper.h"
#include <LiquidCrystal.h>
#include "HatConstants.h"

class HatLib{
    public: 
        HatLib();
        void updatePixels(CRGB (*newPixels));
        void displayPixels();
        void displayMenu();
        void readChangeImageButton();
        void readChangeShiftModeButton();
        void readChangePixelBrightnessButton();

    private:
        CRGB (*originalPixels); //Keep a reference to a CRGB array of length 512. This is so that for whatever reason, someone could load some other rgb data outside the scope of this class, and update the hat design that way.
        uint8_t startingRelativeColumnIndex = 0; //ranges from 0 to 63 for a total of 64 columns
        int startingRowIndex = 0; //represents the idx of the first element of a row - ranges from 0 to 512-64 = 448
        unsigned int getPositiveModulo(unsigned int num, unsigned int modulus);

        //Stores hat's current shift mode
        PIXEL_SHIFT_DIRECTION currVerticalShift = UP_SHIFT;
        PIXEL_SHIFT_DIRECTION currHorizontalShift = RIGHT_SHIFT;
        PIXEL_SHIFT_DIRECTION currShiftDirection = currHorizontalShift;

        String* hatFileNames; //Arduino string array of .HAT files on the SD card
        CRGB pixels[NUM_PIXELS]; //Pixel buffer that stores RGB pixel information obtained from reading a .HAT file on the SD card.
        uint8_t numHatFiles = 0; //Keeps track of how many HAT files are on the SD card's root dir. 
        uint8_t currImageIdx = 0; //Keeps track of the current file being used.

        uint8_t pixelBrightness = 0;

        SDWrapper* sd; //The SD wrapper likes to be initialized on the heap for some reason. Probably has something to do with the inner workings of the SD library? Not a huge deal I suppose. 
};
