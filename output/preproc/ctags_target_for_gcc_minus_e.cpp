# 1 "x:\\My Drive\\Apps\\Hat\\hat\\hat.ino"
# 2 "x:\\My Drive\\Apps\\Hat\\hat\\hat.ino" 2
# 3 "x:\\My Drive\\Apps\\Hat\\hat\\hat.ino" 2

SDWrapper sd;

void setup(){
    Serial.begin(9600);
    while(!Serial){}
    sd = SDWrapper();
    String* s = sd.readRootDirectory();

    for(int i = 0; i < 5; ++i){
            if((s+i)->length() == 0) continue;
            Serial.println(*(s+i));
            Serial.println(i);
    }
}

void loop(){

}
