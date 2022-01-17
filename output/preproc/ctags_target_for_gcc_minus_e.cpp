# 1 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\hat.ino"
# 2 "c:\\Users\\guoob\\Desktop\\Hat\\hat\\hat.ino" 2
HatLib* hatlib; //Store reference to hatlib object, needs to be initialized on heap.

void setup(){
    //Wait for serial monitor to load for debugging purposes.
    Serial.begin(9600);
    while(!Serial){}

    hatlib = new HatLib(); //Initialize hat library
}

void loop(){
    delay(100 /* The number of microseconds per shift*/);
    hatlib->displayPixels();
    hatlib->displayMenu();
    hatlib->readChangeImageButton();
    hatlib->readChangeShiftModeButton();
}
