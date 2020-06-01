void off(void);
void blink(void);

int main(void) {
    init();
    while(1) {
        if(digitalRead(SW_R) == 0) {
            digitalWrite(LED_R, 1);
        } 
        else if(digitalRead(SW_Y) == 0) {
            digitalWrite(LED_Y, 1);
        }
        else if(digitalRead(SW_G) == 0) {
            digitalWrite(LED_G, 1);
        }
        else if(digitalRead(SW_W) == 0) {
            blink();
        }
        off();
    }
    return 0;
}

void init(void) {
    if(wiringPiSetup() == -1) {
        puts("Setup Fail");
        exit(1);
    }   
    pinMode(SW_R, INPUT);
    pinMode(SW_Y, INPUT);
    pinMode(SW_G, INPUT);
    pinMode(SW_W, INPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_Y, OUTPUT);
    pinMode(LED_G, OUTPUT);
    off();
}

void off(void) {
    digitalWrite(LED_R, 0);
    digitalWrite(LED_Y, 0);
    digitalWrite(LED_G, 0);
}

void blink(void) {
    int i=0;

    while(i<3) {
        digitalWrite(LED_R, 1);
        delay(250);
        digitalWrite(LED_R. 0);

        digitalWrite(LED_R, 1);
        delay(250);
        digitalWrite(LED_R. 0);

        digitalWrite(LED_R, 1);
        delay(250);
        digitalWrite(LED_R. 0);
        i++;
    }
}