#line 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\hatlib.cpp"
#include "hatlib.h"

#define NUM_PIXELS				512 //8 x 64 pixels
#define PIXEL_ROWS 				8
#define PIXEL_COLS 				64
#define PIXEL_DATA_PIN 			12
#define PIXEL_BRIGHTNESS 		1 
#define PIXEL_COLOUR_ORDER		GRB

#define LCD_CONTRAST_PIN		3
#define LCD_BACKLIGHT_PIN 		6
#define LCD_RS_PIN				4
#define LCD_E_PIN				5
#define LCD_D4_PIN				10
#define LCD_D5_PIN				9
#define LCD_D6_PIN				8
#define LCD_D7_PIN				7
#define LCD_COLS				16
#define LCD_ROWS				2
#define LCD_BACKLIGHT_TIMEOUT 	5000

#define BUTTON A0 //5 button analog keypad - each button press outputs a different ADC value
#define CHANGE_IMAGE_BUTTON_ADC_VAL 140 
#define CHANGE_STATIC_SHIFT_ADC_VAL 740
#define CHANGE_HORIZONTAL_SHIFT_ADC_VAL 500
#define CHANGE_VERTICAL_SHIFT_ADC_VAL 328


CRGB ledPixelBuffer[NUM_PIXELS];
LiquidCrystal lcd = LiquidCrystal(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

HatLib::HatLib() {
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
	pinMode(LCD_CONTRAST_PIN, OUTPUT);

	FastLED.addLeds<WS2812, PIXEL_DATA_PIN, PIXEL_COLOUR_ORDER>(ledPixelBuffer, NUM_PIXELS);
	FastLED.setBrightness(PIXEL_BRIGHTNESS);

	lcd.begin(LCD_COLS, LCD_ROWS);
	lcd.clear();

	analogWrite(LCD_BACKLIGHT_PIN, 255);
	analogWrite(LCD_CONTRAST_PIN, 108);
}

void HatLib::displayMenu(const char *imageName, PIXEL_SHIFT_DIRECTION currentMode){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(imageName);

	const char *mode = "";
	switch(currentMode){
		case LEFT_SHIFT:
			mode = "LEFT SHIFT";
			break;
		case RIGHT_SHIFT:
			mode = "RIGHT SHIFT";
			break;
		case UP_SHIFT:
			mode = "UP SHIFT";
			break;
		case DOWN_SHIFT:
			mode = "DOWN SHIFT";
			break;
		case NO_SHIFT:
			mode = "STATIC";
			break;
	}

	lcd.setCursor(0, 1);
	lcd.print(mode);
}

void HatLib::updatePixels(CRGB (*newPixels)){
	originalPixels = newPixels;
}

unsigned int HatLib::getPositiveModulo(unsigned int num, unsigned int modulus){
	return ((num % modulus) + modulus) % modulus;
}

//returns true if button is pressed 
bool HatLib::readChangeImageButton(){
	//Check if analog read val is around around ADC button range
	return abs(analogRead(BUTTON) - CHANGE_IMAGE_BUTTON_ADC_VAL) <= 10; 
	//To Do: account for debounce
}

//returns true if button is pressed 
void HatLib::readChangeShiftModeButton(PIXEL_SHIFT_DIRECTION &currHorizontalShift, PIXEL_SHIFT_DIRECTION &currVerticalShift, PIXEL_SHIFT_DIRECTION &currDir){
	//Check if analog read val is around around ADC button range
	if(abs(analogRead(BUTTON) - CHANGE_HORIZONTAL_SHIFT_ADC_VAL) <= 10){
		currHorizontalShift = currHorizontalShift == LEFT_SHIFT ? RIGHT_SHIFT : LEFT_SHIFT;
		currDir = currHorizontalShift;
	}; 

	if(abs(analogRead(BUTTON) - CHANGE_VERTICAL_SHIFT_ADC_VAL) <= 10){
		currVerticalShift = currVerticalShift == UP_SHIFT ? DOWN_SHIFT : UP_SHIFT;
		currDir = currVerticalShift;
	}; 

	if(abs(analogRead(BUTTON) - CHANGE_STATIC_SHIFT_ADC_VAL) <= 10){
		currDir = NO_SHIFT;
	}; 
	//To Do: account for debounce
}

void HatLib::displayPixels(PIXEL_SHIFT_DIRECTION shiftDirection){
	int jumpDirection; //for up/down shift direction 
	int currPos = 0;
	int absoluteIndex; //idx that keeps track of our current position when we jump
	bool jumpRight = true; //general direction of jumps
	
	 switch(shiftDirection){
	 	case RIGHT_SHIFT:
	 		//shift start by 1 left - get a positive modulo
	 		startingRelativeColumnIndex = getPositiveModulo(startingRelativeColumnIndex - 1, PIXEL_COLS);
			absoluteIndex = startingRelativeColumnIndex;
	 		break;
	 	case LEFT_SHIFT:
	 		//shift start by 1 right - get a positive modulo
	 		startingRelativeColumnIndex = getPositiveModulo(startingRelativeColumnIndex + 1, PIXEL_COLS);
			absoluteIndex = startingRelativeColumnIndex;
	 		break;
		case UP_SHIFT:
			jumpDirection = 1;
			absoluteIndex = startingRowIndex;
			break;
		case DOWN_SHIFT:
			jumpDirection = -1;
			absoluteIndex = startingRowIndex;
			break;
		default:
			absoluteIndex = 0;
			break;
	 }

	 while(true){
	 	//if currPos exceeds numPixels, we are done
	 	if(currPos >= NUM_PIXELS) break; 
		
		//I dislike having to use 2 buffers but it works and makes things easy. A future student could work on designing an algorithm 
		//that uses only one CRGB array and pass it as a reference to FastLED lib - memory usage would effectively go down by ~20-30% 
		//Anywyay, update the pixels based on the abs idx value that's used to jump around in the original array
	 	ledPixelBuffer[currPos].red = originalPixels[absoluteIndex][0];
	 	ledPixelBuffer[currPos].green = originalPixels[absoluteIndex][1];
	 	ledPixelBuffer[currPos].blue = originalPixels[absoluteIndex][2];
	 	++currPos; 

		//Code for handling up and down shifts
		if(shiftDirection == UP_SHIFT || shiftDirection == DOWN_SHIFT){
			
			//Check to see if the next jump brings us back to the starting col in the row we began to start jumping from in a particular direction
			if(getPositiveModulo(absoluteIndex + 
			(PIXEL_COLS * (jumpRight ? 1 * jumpDirection : -1 * jumpDirection)), NUM_PIXELS) == startingRowIndex) {
				//If so, we increment the abs index one over (positive modulo as well!) and jump in the reverse direction
				//And also update the starting row we started jumping from in that direction
				absoluteIndex = getPositiveModulo(absoluteIndex + 1, NUM_PIXELS);
				startingRowIndex = absoluteIndex;
				jumpRight = !jumpRight;
			}

			//Get positive modulo when we jump left or right by col number mod num pixels
			//the direction of jump depends on if we are jumping right or not, and the shift direction 
			else absoluteIndex = getPositiveModulo(absoluteIndex + 
			(PIXEL_COLS * (jumpRight ? 1 * jumpDirection : -1 * jumpDirection)), NUM_PIXELS);
		}

		//Code for handling right/left/no shifts
		else{
			//shift index one over if the next position is invalid
			if(absoluteIndex + PIXEL_COLS >= NUM_PIXELS && jumpRight || absoluteIndex - PIXEL_COLS < 0 && !jumpRight){
				++absoluteIndex; 

				//check if the index is out of bounds of array - if so, roll back
				if(absoluteIndex == NUM_PIXELS) absoluteIndex = absoluteIndex - PIXEL_COLS; //roll back to beginning of last row
				if(absoluteIndex == PIXEL_COLS) absoluteIndex = 0; //roll back to beginning of first row
				jumpRight = !jumpRight; //reverse jump direction
			}
			else absoluteIndex += PIXEL_COLS * (jumpRight ? 1 : -1); //continue jumping
		}
	}	

	//After writing the new pixels to the led buffer, output the buffer data for the strip to display 
	FastLED.show();
}