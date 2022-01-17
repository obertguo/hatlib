#line 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\test\\hatlib.cpp"
#include "hatlib.h"

#define NUM_PIXELS				512 //8 x 64 pixels
#define PIXEL_ROWS 				8
#define PIXEL_COLS 				64
#define PIXEL_DATA_PIN 			12
#define PIXEL_BRIGHTNESS 		5 
#define PIXEL_COLOUR_ORDER		GRB

// #define LCD_CONTRAST_PIN		A0
// #define LCD_BACKLIGHT_PIN 		A1
// #define LCD_RS_PIN				12
// #define LCD_E_PIN				11
// #define LCD_D4_PIN				10
// #define LCD_D5_PIN				9
// #define LCD_D6_PIN				8
// #define LCD_D7_PIN				7
// #define LCD_COLS				16
// #define LCD_ROWS				2
// #define LCD_BACKLIGHT_TIMEOUT 	5000

// #define LCD_CONTRAST_PIN		3
// #define LCD_BACKLIGHT_PIN 		6
// #define LCD_RS_PIN				4
// #define LCD_E_PIN				5
// #define LCD_D4_PIN				10
// #define LCD_D5_PIN				9
// #define LCD_D6_PIN				8
// #define LCD_D7_PIN				7
// #define LCD_COLS				16
// #define LCD_ROWS				2
// #define LCD_BACKLIGHT_TIMEOUT 	5000

// #define PB_UP_PIN 				5
// #define PB_DOWN_PIN				6

#define SHIFT_RATE				120 //the duration the image will appear in microseconds before shifting 

// CRGB pixelArray[NUM_PIXELS];


//LCD Stuff
// LiquidCrystal lcd = LiquidCrystal(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

HatLib::HatLib() {
    // pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
	// pinMode(LCD_CONTRAST_PIN, OUTPUT);

	// pinMode(PB_UP_PIN, INPUT_PULLUP);
	// pinMode(PB_DOWN_PIN, INPUT_PULLUP);

	FastLED.addLeds<WS2812, PIXEL_DATA_PIN, PIXEL_COLOUR_ORDER>(currentPixels, NUM_PIXELS);
	FastLED.setBrightness(PIXEL_BRIGHTNESS);

	// lcd.begin(LCD_COLS, LCD_ROWS);
	// lcd.clear();
}


void HatLib::updatePixels(CRGB (&newPixels)[512]){
	currentPixels = newPixels;
}

// void HatLib::displayPixels(PIXEL_SHIFT_DIRECTION shiftDirection){
// 	 switch(shiftDirection){
// 	 	case RIGHT_SHIFT:
// 	 		//shift start by 1 left - get a positive modulo
// 	 		startingRelativeColumnIndex = (((startingRelativeColumnIndex - 1) % PIXEL_COLS) + PIXEL_COLS) % PIXEL_COLS;
// 	 		break;
// 	 	case LEFT_SHIFT:
// 	 		//shift start by 1 right - get a positive modulo
// 	 		startingRelativeColumnIndex = (((startingRelativeColumnIndex + 1) % PIXEL_COLS) + PIXEL_COLS) % PIXEL_COLS;
// 	 		break;
// 	 	//Otherwise, a NO_SHIFT is specified and nothing needs to happen
// 	 }

// 	 int currPos = 0;
// 	 int absoluteIndex = startingRelativeColumnIndex;
// 	 bool jumpRight = true;

// 	 while(true){
// 	 	//if currPos exceeds numPixels, we are done
// 	 	if(currPos >= NUM_PIXELS) break; 
		
// 	 	pixelArray[currPos].red = currentPixels[absoluteIndex][0];
// 	 	pixelArray[currPos].green = currentPixels[absoluteIndex][1];
// 	 	pixelArray[currPos].blue = currentPixels[absoluteIndex][2];
// 	 	++currPos; 

// 	 	if(jumpRight){
// 	 		if(absoluteIndex + PIXEL_COLS >= NUM_PIXELS) {
// 	 			++absoluteIndex;

// 	 			//If after adding 1, the abs index is out of bounds of the array, we roll back to the beginning el of that row
// 	 			if(absoluteIndex == NUM_PIXELS) absoluteIndex = absoluteIndex - PIXEL_COLS; //roll back
				
// 	 			jumpRight = !jumpRight;
// 	 		}
// 	 		else absoluteIndex += PIXEL_COLS;
// 	 	}

// 	 	else{
// 	 		if(absoluteIndex - PIXEL_COLS < 0) {
// 	 			++absoluteIndex;
// 	 			//If after adding 1, the abs index is out of bounds of the first row, we roll back to the beginning	
// 	 			if(absoluteIndex == PIXEL_COLS) absoluteIndex = 0; //roll back
				
// 	 			jumpRight = !jumpRight;
// 	 		}
// 	 		else absoluteIndex -= PIXEL_COLS;
// 	 	}
// 	 }	

// 	//After pushing out the new pixels, get the strip to display 
// 	FastLED.show();
// }


//Shortened code
void HatLib::displayPixels(PIXEL_SHIFT_DIRECTION shiftDirection){
	 switch(shiftDirection){
	 	case RIGHT_SHIFT:
	 		//shift start by 1 left - get a positive modulo
	 		startingRelativeColumnIndex = (((startingRelativeColumnIndex - 1) % PIXEL_COLS) + PIXEL_COLS) % PIXEL_COLS;
	 		break;
	 	case LEFT_SHIFT:
	 		//shift start by 1 right - get a positive modulo
	 		startingRelativeColumnIndex = (((startingRelativeColumnIndex + 1) % PIXEL_COLS) + PIXEL_COLS) % PIXEL_COLS;
	 		break;
	 	//Otherwise, a NO_SHIFT is specified and nothing needs to happen
	 }

	 int currPos = 0;
	 int absoluteIndex = startingRelativeColumnIndex;
	 bool jumpRight = true;

	 while(true){
	 	//if currPos exceeds numPixels, we are done
	 	if(currPos >= NUM_PIXELS) break; 
		
		//I dislike having to use 2 buffers - this can be massively improved on
	 	currentPixels[currPos].red = currentPixels[absoluteIndex][0];
	 	currentPixels[currPos].green = currentPixels[absoluteIndex][1];
	 	currentPixels[currPos].blue = currentPixels[absoluteIndex][2];
		
	 	++currPos; 

		//shift index one over if the next position is invalid
		if(absoluteIndex + PIXEL_COLS >= NUM_PIXELS || absoluteIndex - PIXEL_COLS < 0){
			++absoluteIndex; 

			//check if the index is out of bounds of array - if so, roll back
			if(absoluteIndex == NUM_PIXELS) absoluteIndex = absoluteIndex - PIXEL_COLS; //roll back to beginning of last row
			if(absoluteIndex == PIXEL_COLS) absoluteIndex = 0; //roll back to beginning of first row
			jumpRight = !jumpRight; //reverse jump direction
		}
		else{
			//continue jumping
			absoluteIndex += PIXEL_COLS * jumpRight == 0 ? -1 : 1; 
		}
	 }	

	//After pushing out the new pixels, get the strip to display 
	FastLED.show();
}