#include "hatlib.h"
HatLib* hatlib; //Store reference to hatlib object, needs to be initialized on heap.

void setup(){
    //Wait for serial monitor to load for debugging purposes.
    Serial.begin(9600);
    while(!Serial){}

    hatlib = new HatLib(); //Initialize hat library
}

void loop(){
    delay(PIXEL_SHIFT_RATE);
    hatlib->displayPixels();
    hatlib->displayMenu();
    hatlib->readChangeImageButton();
    hatlib->readChangeShiftModeButton();    
    hatlib->readChangePixelBrightnessButton();
}
