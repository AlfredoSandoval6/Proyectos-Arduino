/*#define LED 0
#define SENSOR 8

void setup(){

    pinMode(SENSOR, INPUT);
    pinMode(LED, OUTPUT);

    Serial.begin(9600);

}

void loop(){

    lect_sen();

    delay(1);
}

void lect_sen(){
    int lect_sen = 0;

    lect_sen = analogRead(SENSOR);

    if(lect_sen < 455){

        digitalWrite(LED, HIGH);
        Serial.println("El sensor está detectando.");

    } else{

        digitalWrite(LED, LOW);
        Serial.println("El sensor NO está detectando.");

    }
}
*/

#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define SENSOR 8

void setup(){

    pinMode(SENSOR, INPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);

    Serial.begin(9600);

}

void loop(){

    lectsen_encLed();

    delay(1);
}

void lectsen_encLed(){
    int lect_sen = 0;

    lect_sen = analogRead(SENSOR);

    if(lect_sen > 0){

        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);

    }else if(lect_sen > 330){

        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);

    }else if(lect_sen > 660){

        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, LOW);

    }else if(lect_sen > 1023){

        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);

    }
}
