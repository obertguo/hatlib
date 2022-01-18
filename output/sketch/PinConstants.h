#line 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\PinConstants.h"
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

#define BUTTON A0 //5 button analog keypad - each button press outputs a different ADC value
#define CHANGE_IMAGE_BUTTON_ADC_VAL 140 
#define CHANGE_STATIC_SHIFT_ADC_VAL 740
#define CHANGE_HORIZONTAL_SHIFT_ADC_VAL 500
#define CHANGE_VERTICAL_SHIFT_ADC_VAL 328
#define CHANGE_PIXEL_BRIGHTNESS_ADC_VAL 0