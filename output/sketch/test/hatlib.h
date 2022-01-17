#line 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\test\\hatlib.h"
#include <FastLED.h>
#include <LiquidCrystal.h>

enum PIXEL_SHIFT_DIRECTION{
    NO_SHIFT,
    RIGHT_SHIFT,
    LEFT_SHIFT
};

class HatLib{
    public: 
        HatLib();
        void initialize();
        void updatePixels(CRGB (&newpixels)[512]);
        void displayPixels(PIXEL_SHIFT_DIRECTION SHIFT_DIRECTION);

    private:
        CRGB (*currentPixels) = nullptr; //a pointer to an array of len 3
        uint8_t startingRelativeColumnIndex = 0;
};