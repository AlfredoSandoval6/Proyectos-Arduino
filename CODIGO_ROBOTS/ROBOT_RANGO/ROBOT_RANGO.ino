//

#define RAN_SP 70
#define SFLL PIND, 1
#define SFL PIND, 1
#define SF PIND, 1
#define SFR PIND, 1
#define SFLR PIND, 1
#define SP_L A0
#define SP_R A1
#define MOTL PORTD, 1
#define MOTR PORTD, 1
#define PWML 1
#define PWMR 1
#define PIN_SGY_1 PIND, 1
#define PIN_SGY_2 PIND, 1
#define PIN_LED_SGY_1 PORTD, 1
#define PIN_LED_SGY_2 PORTD, 1
#define ARRANCADOR PIND, 1
#define realFastRead(pin, numPin) (((pin) >> (numPin)) & 0x01)
#define fastRead(pin_numPin) realFastRead(pin_numPin)
#define realFastWrite(port, numPin, value) ((value) ? ((port) |= (1U << (numPin))) : ((port) &= ~(1U << (numPin))))
#define fastWrite(port_numPin, value) realFastWrite(port_numPin, value)
#define READ_ARR (fastRead(ARRANCADOR))
#define LECT_ARR(state) if(READ_ARR == state){if(!state) if((fastRead(MOTL)) && (fastRead(MOTR))) paro(); return;}
#define espera(caso, timeLapse) {\
            int beginMillis = millis();\
            if(caso){\
            do{\
                LECT_ARR(LOW)\
            }while((millis() - beginMillis) <= timeLapse);\
            } else{\
            do{\
                LECT_ARR(LOW)\
                lectura();\
                if(sens) return;\
            }while((millis() - beginMillis) <= timeLapse);;\
            }\
/**//**/}

byte sens, sgy;
bool izq = false, der = false;

void setup(){
    DDRB;
    DDRC;
    DDRD;
    paro();

    sens = sgy = 0;
}

void loop(){
    while(1){
        secLeds();

        if(READ_ARR) estrategia();
    }
}

void lectura(){
    (fastRead(SFLL)) ? sens |= (1U << 0) : sens &= ~(1U << 0);
    (fastRead(SFL)) ? sens |= (1U << 1) : sens &= ~(1U << 1);
    (fastRead(SF)) ? sens |= (1U << 2) : sens &= ~(1U << 2);
    (fastRead(SFR)) ? sens |= (1U << 3) : sens &= ~(1U << 3);
    (fastRead(SFLR)) ? sens |= (1U << 4) : sens &= ~(1U << 4);
    (analogRead(SP_L) < 70) ? sens |= (1U << 5) : sens &= ~(1U << 5);
    (analogRead(SP_R) < 70) ? sens |= (1U << 6) : sens &= ~(1U << 6);

    if((bitRead(sens, 0)) && (!bitRead(sens, 4))) izq = true;
    else if((!bitRead(sens, 0)) && (bitRead(sens, 4))) der = true;
    else if((bitRead(sens, 0)) && (bitRead(sens, 4))) izq = der = false;
}

void secLeds(){
    int beginMillis = 0;

    do{
        (fastRead(PIN_SGY_1)) ? sgy |= (1U << 0) : sgy &= ~(1U << 1);
        (fastRead(PIN_SGY_2)) ? sgy |= (1U << 0) : sgy &= ~(1U << 1);
        LECT_ARR(HIGH) beginMillis = millis();

        switch(sgy){
        case 0B00000000:
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_1, HIGH);
            do{
                LECT_ARR(HIGH)
                if(fastRead(PIN_SGY_1) != bitRead(sgy, 0)) return; if(fastRead(PIN_SGY_2) != bitRead(sgy, 1)) return;
            }while((millis() - beginMillis) <= 500);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, LOW); fastWrite(PIN_LED_SGY_1, LOW);
            do{
                LECT_ARR(HIGH)
                if(fastRead(PIN_SGY_1) != bitRead(sgy, 0)) return; if(fastRead(PIN_SGY_2) != bitRead(sgy, 1)) return;
            }while((millis() - beginMillis) <= 500);
        break;
        
        case 0B00000001:
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_1, LOW);
            do{
                LECT_ARR(HIGH)
                if(fastRead(PIN_SGY_1) != bitRead(sgy, 0)) return; if(fastRead(PIN_SGY_2) != bitRead(sgy, 1)) return;
            }while((millis() - beginMillis) <= 400);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, LOW); fastWrite(PIN_LED_SGY_1, HIGH);
            do{
                LECT_ARR(HIGH)
                if(fastRead(PIN_SGY_1) != bitRead(sgy, 0)) return; if(fastRead(PIN_SGY_2) != bitRead(sgy, 1)) return;
            }while((millis() - beginMillis) <= 400);
        break;

        case 0B00000010:
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_1, HIGH);
            do{
                LECT_ARR(HIGH)
                if(fastRead(PIN_SGY_1) != bitRead(sgy, 0)) return; if(fastRead(PIN_SGY_2) != bitRead(sgy, 1)) return;
            }while((millis() - beginMillis) <= 300);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_1, LOW);
            do{
                LECT_ARR(HIGH)
                if(fastRead(PIN_SGY_1) != bitRead(sgy, 0)) return; if(fastRead(PIN_SGY_2) != bitRead(sgy, 1)) return;
            }while((millis() - beginMillis) <= 300);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, LOW); fastWrite(PIN_LED_SGY_1, LOW);
            do{
                LECT_ARR(HIGH)
                if(fastRead(PIN_SGY_1) != bitRead(sgy, 0)) return; if(fastRead(PIN_SGY_2) != bitRead(sgy, 1)) return;
            }while((millis() - beginMillis) <= 300);
        break;

        case 0B00000011:
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_1, HIGH);
            do{
                LECT_ARR(HIGH)
                if(fastRead(PIN_SGY_1) != bitRead(sgy, 0)) return; if(fastRead(PIN_SGY_2) != bitRead(sgy, 1)) return;
            }while((millis() - beginMillis) <= 200);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, LOW); fastWrite(PIN_LED_SGY_1, LOW);
            do{
                LECT_ARR(HIGH)
                if(fastRead(PIN_SGY_1) != bitRead(sgy, 0)) return; if(fastRead(PIN_SGY_2) != bitRead(sgy, 1)) return;
            }while((millis() - beginMillis) <= 200);
        break;

        default:
        break;
        }
    }while(!READ_ARR);
}

void estrategia(){
    switch(sgy){
        case 0B00000000:
            sgy1();
        break;

        case 0B00000001:
            sgy2();
        break;

        case 0B00000010:
            sgy3();
        break;

        case 0B00000011:
            sgy4();
        break;

        default:
        break;
    }
}

void sgy1(){
    izquierda(240, 240);
    
}

void sgy2(){
    izquierda(240, 240);
    
}

void sgy3(){
    izquierda(240, 240);
    
}

void sgy4(){
    izquierda(240, 240);
    
}

void busqueda(){
    do{
    adelante(30, 30);
    espera(false, 10000);
    }while(READ_ARR);
}

void izquierda(byte PWMI, byte PWMD){
    fastWrite(MOTL, LOW);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, HIGH);
    analogWrite(PWMR, PWMD);
}

void derecha(byte PWMI, byte PWMD){
    fastWrite(MOTL, LOW);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, LOW);
    analogWrite(PWMR, PWMD);
}

void adelante(byte PWMI, byte PWMD){
    fastWrite(MOTL, HIGH);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, HIGH);
    analogWrite(PWMR, PWMD);
}

void paro(){
    fastWrite(MOTL, LOW);
    analogWrite(PWML, 0);
    fastWrite(MOTR, LOW);
    analogWrite(PWMR, 0);
}