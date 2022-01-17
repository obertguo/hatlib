#include <FastLED.h>
#include <LiquidCrystal.h>
#define NUM_PIXELS				512

enum PIXEL_SHIFT_DIRECTION{
    NO_SHIFT,
    RIGHT_SHIFT,
    LEFT_SHIFT,
    UP_SHIFT,
    DOWN_SHIFT
};

class HatLib{
    public: 
        HatLib();
        void initialize();
        void updatePixels(CRGB (*newPixels));
        void displayPixels(PIXEL_SHIFT_DIRECTION SHIFT_DIRECTION);
        void displayMenu(const char *imageName, PIXEL_SHIFT_DIRECTION currentMode);
        bool readChangeImageButton();
        void readChangeShiftModeButton(PIXEL_SHIFT_DIRECTION &currHorizontalShift, PIXEL_SHIFT_DIRECTION &currVerticalShift, PIXEL_SHIFT_DIRECTION &currDir);

    private:
        CRGB (*originalPixels); //a reference to a CRGB array of length 512
        uint8_t startingRelativeColumnIndex = 0; //ranges from 0 to 63 for a total of 64 columns
        int startingRowIndex = 0; //represents the idx of the first element of a row - ranges from 0 to 512-64 = 448
        unsigned int getPositiveModulo(unsigned int num, unsigned int modulus);
};