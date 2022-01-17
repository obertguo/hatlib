#line 1 "x:\\My Drive\\Apps\\Hat\\hat\\hatlib.h"
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
        void updatePixels(uint8_t newPixels[][3]);
        void displayPixels(PIXEL_SHIFT_DIRECTION SHIFT_DIRECTION);

    private:
        uint8_t (*currentPixels)[3]; //a pointer to an array of len 3
        uint8_t startingRelativeColumnIndex = 0;
};