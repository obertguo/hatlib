enum PIXEL_SHIFT_DIRECTION{
    NO_SHIFT,
    RIGHT_SHIFT,
    LEFT_SHIFT,
    UP_SHIFT,
    DOWN_SHIFT
};

#define NUM_PIXELS 512 //Hat contains 64 * 8 pixels
#define PIXEL_SHIFT_RATE 100 // The number of microseconds per shift
#define MAX_PATTERNS 6 // The maximum pattern files we will load. Can be set to whatever positive integer you'd like of reasonable size before the Arduino encounters memory issues. 

#define LCD_BACKLIGHT_VAL 255
#define LCD_CONTRAST_VAL 108