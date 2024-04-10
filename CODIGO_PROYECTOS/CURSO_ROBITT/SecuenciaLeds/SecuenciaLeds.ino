#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5

void setup(){
    
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);

}

void loop(){

    secuenciaLeds();

}

void secuenciaLeds(){

    for(int i = LED2; i <= LED4; ++i) digitalWrite(i, HIGH);
    delay(250);
    for(int i = LED1; i <= LED3 ; ++i) digitalWrite(i, LOW);
    delay(250);

    digitalWrite(LED1, HIGH);
    for(int i = LED2; i <= LED4; ++i){

        digitalWrite(i, HIGH);
        digitalWrite((i - 1), LOW);
        delay(250);

    }

    for(int i = LED3; i <= LED1; --i){

        digitalWrite(i, HIGH);
        digitalWrite((i + 1), LOW);
        delay(250);

    }

}
