#include "hatlib.h"
#include "PinConstants.h"

CRGB ledPixelBuffer[NUM_PIXELS];
LiquidCrystal lcd = LiquidCrystal(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

HatLib::HatLib() {
	//Initialize LCD and RGB strip
  	pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
	pinMode(LCD_CONTRAST_PIN, OUTPUT);

	FastLED.addLeds<WS2812, PIXEL_DATA_PIN, PIXEL_COLOUR_ORDER>(ledPixelBuffer, NUM_PIXELS);
	FastLED.setBrightness(pixelBrightness);

	lcd.begin(LCD_COLS, LCD_ROWS);
	lcd.clear();

	analogWrite(LCD_BACKLIGHT_PIN, LCD_BACKLIGHT_VAL);
	analogWrite(LCD_CONTRAST_PIN, LCD_CONTRAST_VAL);

	//Initialize SD card and load HAT files and pixel data of first HAT file.
	sd = new SDWrapper(); //Initialize SD library.
	hatFileNames = sd->readRootDirectory(); //Read hat files in root dir, the filenames are stored as an array of Arduino strings. 

	//Determine the number of hat files we've found by iterating through the filename array starting at the beginning. Once an empty string is encountered, we've found the number of HAT files.
	for(uint8_t i = 0; i < MAX_PATTERNS; ++i){ 
       if((hatFileNames+i)->length() == 0) break;
       ++numHatFiles;
       Serial.println(*(hatFileNames+i)); //Output .HAT file names on SD card for debugging purposes.
    }

	sd->readFile(hatFileNames[0], pixels); //Read data stored on the first hat file, the RGB data will be stored to the pixels array. (The pixels array is passed by reference)
	this->updatePixels(pixels); //Update hat's internal pixel reference so that it can read rgb data at the specified memory address of the pixel buffer
}

void HatLib::displayMenu(){
	const char* fileName = hatFileNames[currImageIdx].c_str();

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(fileName);

	const char *currShiftDirectionName = "";
	switch(currShiftDirection){
		case LEFT_SHIFT:
			currShiftDirectionName = "LEFT SHIFT";
			break;
		case RIGHT_SHIFT:
			currShiftDirectionName = "RIGHT SHIFT";
			break;
		case UP_SHIFT:
			currShiftDirectionName = "UP SHIFT";
			break;
		case DOWN_SHIFT:
			currShiftDirectionName = "DOWN SHIFT";
			break;
		case NO_SHIFT:
			currShiftDirectionName = "STATIC";
			break;
	}

	lcd.setCursor(0, 1);
	lcd.print(currShiftDirectionName);
	lcd.setCursor(12, 1);
	lcd.print(pixelBrightness);
}

void HatLib::updatePixels(CRGB (*newPixels)){
	originalPixels = newPixels;
}

unsigned int HatLib::getPositiveModulo(unsigned int num, unsigned int modulus){
	return ((num % modulus) + modulus) % modulus;
}

//returns true if button is pressed 
void HatLib::readChangeImageButton(){
	//Check if analog read val is around around ADC button range (+/- 10)
	if(abs(analogRead(BUTTON) - CHANGE_IMAGE_BUTTON_ADC_VAL) <= 10){
		//Shift image index one over in a modulo fashion, and update pixel data.
		currImageIdx = (currImageIdx + 1) % numHatFiles;
		const char* fileName = hatFileNames[currImageIdx].c_str();
		sd->readFile(fileName, pixels);
	}
	//To Do: account for debounce
}

//returns true if button is pressed 
void HatLib::readChangeShiftModeButton(){
	//Check if analog read val is around around ADC button range (+/- 10)
	if(abs(analogRead(BUTTON) - CHANGE_HORIZONTAL_SHIFT_ADC_VAL) <= 10){
		currHorizontalShift = currHorizontalShift == LEFT_SHIFT ? RIGHT_SHIFT : LEFT_SHIFT;
		currShiftDirection = currHorizontalShift;
	}

	else if(abs(analogRead(BUTTON) - CHANGE_VERTICAL_SHIFT_ADC_VAL) <= 10){ 
		currVerticalShift = currVerticalShift == UP_SHIFT ? DOWN_SHIFT : UP_SHIFT;
		currShiftDirection = currVerticalShift;
	}

	else if(abs(analogRead(BUTTON) - CHANGE_STATIC_SHIFT_ADC_VAL) <= 10){
		currShiftDirection = NO_SHIFT;
	}
	//To Do: account for debounce
}

void HatLib::readChangePixelBrightnessButton(){
	if(abs(analogRead(BUTTON) - CHANGE_PIXEL_BRIGHTNESS_ADC_VAL) <= 10){
		pixelBrightness = getPositiveModulo(pixelBrightness + 10, 255); //increase bringtness by 10 each press and wrap around at 255.
		FastLED.setBrightness(pixelBrightness);
	}
}

void HatLib::displayPixels(){
	int jumpDirection; //for up/down shift direction 
	int currPos = 0;
	int absoluteIndex; //idx that keeps track of our current position when we jump
	bool jumpRight = true; //general direction of jumps
	
	 switch(currShiftDirection){
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
		if(currShiftDirection == UP_SHIFT || currShiftDirection == DOWN_SHIFT){
			
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
