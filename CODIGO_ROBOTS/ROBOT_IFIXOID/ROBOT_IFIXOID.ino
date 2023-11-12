//ROBOT IFIXOID
#include "Servo.h"

#define SFLL PIND, 1
#define SFCL PIND, 1
#define SFL PIND, 1
#define SF PIND, 1
#define SFR PIND, 1
#define SFCR PIND, 1
#define SFLR PIND, 1
#define SP_L A1
#define SP_R A2
#define MOTL PORTD, 1
#define MOTR PORTD, 1
#define PWML 1
#define PWMR 1
#define PIN_SGY_1 PIND, 1
#define PIN_SGY_2 PIND, 1
#define LED_SGY_1 PORTD, 0
#define LED_SGY_2 PORTD, 1
#define ARRANCADOR PINB, 1
#define realFastRead(pin, numPin) (((pin) >> (numPin)) & 0x01)
#define fastRead(pin_numPin) realFastRead(pin_numPin)
#define realFastWrite(port, numPin, val) ((val) ? bitSet(port, numPin) : bitClear(port, numPin))
#define fastWrite(port_numPin, val) realFastWrite(port_numPin, val)
#define LECT_ARR(state) (fastRead(ARRANCADOR) == state)
#define ACT_ARR(state) if(LECT_ARR(state)){\
            if(!state){paro(1); banderaL.write(0); banderaR.write(90);}; return;\
/**//**/}
#define espera(caso, state, timeLapse) {\
            int beginMillis = millis();\
            if(caso){\
                do{\
                    ACT_ARR(state)\
                }while((millis() - beginMillis) <= timeLapse);\
            } else{\
                do{\
                    ACT_ARR(state)\
                    lectura();\
                    if(sens) return;\
                }while((millis() - beginMillis) <= timeLapse);\
            }\
/**//**/}
//NOTA: SIENDO QUE ESTA FUNCIÓN ACABA CON UN IF, PUEDE SER CONTINUADA CON UN ELSE. OTROS MÉTODOS SERÁN FACTIBLES.
#define SEARCH_AND_FLOOR_DODGE lectura(); if(sens == 0) busqueda(); ACT_ARR(LOW)\
        if(bitRead(sens, 7) || bitRead(sens, 8)){\
            atras(255, 255, 120);\
            if(bitRead(sens, 7) && bitRead(sens, 8)){\
                izquierda(false, 240, 240, 120);\
            } else{\
                if(bitRead(sens, 7)){\
                    izquierda(false, 240, 240, 120);\
                }\
                if(bitRead(sens, 8)){\
                    derecha(false, 240, 240, 120);\
                }\
            }\
/**//**/}

int sens;
byte sgy;
bool izq = false, der = false;
Servo banderaL, banderaR;

void secLeds(void);
void lectura(void);
void estrategia(void);
void busqueda(void);
void sgy1(void);
void sgy2(void);
void sgy3(void);
void sgy4(void);
void izquierda(bool caso, byte PWMI, byte PWMD, int T);
void derecha(bool caso, byte PWMI, byte PWMD, int T);
void adelante(bool caso, byte PWMI, byte PWMD, int T);
void atras(byte PWMI, byte PWMD, int T);
void paro(int T);

void setup(){
    DDRB;
    DDRD;
    paro(1);
    delay(3000);
    //BANDERAS ORIGINALMENTE CERRADAS
    banderaL.attach(10); banderaR.attach(9);
    banderaL.write(0); banderaR.write(90);

    sens = sgy = 0;
}

void loop(){
    while(1){
        secLeds();

        if(LECT_ARR(HIGH)) estrategia();
    }
}

void secLeds(void){
    int timeLapse = 0;
    byte steLeds = 0;

    if(fastRead(PIN_SGY_1)) sgy |= (1 << 0); else sgy &= ~(1 << 0);
    if(fastRead(PIN_SGY_2)) sgy |= (1 << 1); else sgy &= ~(1 << 1);
    ACT_ARR(HIGH)

    switch(sgy){
        case 0B00000000:
            steLeds = 0B00000011; timeLapse = 500;
        break;

        case 0B00000001:
            steLeds = 0B00001001; timeLapse = 400;
        break;

        case 0B00000010:
            steLeds = 0B00000111; timeLapse = 300;
        break;

        case 0B00000011:
            steLeds = 0B00000011; timeLapse = 200;
        break;

        default:
        break;
    }

    fastWrite(LED_SGY_1, bitRead(steLeds, 0)); fastWrite(LED_SGY_2, bitRead(steLeds, 1));
    espera(true, HIGH, timeLapse);
    fastWrite(LED_SGY_1, bitRead(steLeds, 2)); fastWrite(LED_SGY_2, bitRead(steLeds, 3));
    espera(true, HIGH, timeLapse);
}

void lectura(void){
    if(fastRead(SFLL)) sens |= (1 << 0); else sens &= ~(1 << 0);
    if(fastRead(SFCL)) sens |= (1 << 1); else sens &= ~(1 << 1);
    if(fastRead(SFL)) sens |= (1 << 2); else sens &= ~(1 << 2);
    if(fastRead(SF)) sens |= (1 << 3); else sens &= ~(1 << 3);
    if(fastRead(SFR)) sens |= (1 << 4); else sens &= ~(1 << 4);
    if(fastRead(SFCR)) sens |= (1 << 5); else sens &= ~(1 << 5);
    if(fastRead(SFLR)) sens |= (1 << 6); else sens &= ~(1 << 6);
    if(analogRead(SP_L) < 100) sens |= (1 << 7); else sens &= ~(1 << 7);
    if(analogRead(SP_R) < 100) sens |= (1 << 8); else sens &= ~(1 << 8);

    if((bitRead(sens, 0)) && (!bitRead(sens, 6))) izq = true;
    else if((!bitRead(sens, 0)) && (bitRead(sens, 6))) der = true;
    else if((bitRead(sens, 0)) && (bitRead(sens, 6))) izq = der = false;
}

void estrategia(void){
    lectura();
    //BANDERAS ABIERTAS
    banderaL.write(90);
    banderaR.write(0);

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

void sgy1(void){
    if(bitRead(sens, 0)) atras(186, 248, 120);
    else if(bitRead(sens, 6)) atras(248, 186, 120);

    do{
        SEARCH_AND_FLOOR_DODGE
        else{

        }
    }while(LECT_ARR(HIGH));
}

void sgy2(void){
    adelante(true, 31, 248, 120);

    do{
        SEARCH_AND_FLOOR_DODGE
        else{

        }
    }while(LECT_ARR(HIGH));
}

void sgy3(void){
    derecha(true, 248, 248, 30);
    adelante(false, 248, 248, 200);
    izquierda(false, 248, 248, 200);

    do{
        SEARCH_AND_FLOOR_DODGE
        else{

        }
    }while(LECT_ARR(HIGH));
}

void sgy4(void){
    atras(248, 248, 300);

    do{
        SEARCH_AND_FLOOR_DODGE
        else{

        }
    }while(LECT_ARR(HIGH));
}

void busqueda(void){
    if(izq || der){
        if(izq) izquierda(false, 240, 240, 150);
        else if(der) derecha(false, 240, 240, 150);
        izq = der = false;
    }

    ACT_ARR(LOW)
    if(!sens) return;

    do{
        adelante(false, 30, 30, 10);
        ACT_ARR(LOW)
    }while(sens == 0);
}

void izquierda(bool caso, byte PWMI, byte PWMD, int T){
    fastWrite(MOTL, LOW);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, HIGH);
    analogWrite(PWMR, PWMD);
    espera(caso, LOW, T);
}

void derecha(bool caso, byte PWMI, byte PWMD, int T){
    fastWrite(MOTL, HIGH);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, LOW);
    analogWrite(PWMR, PWMD);
    espera(caso, LOW, T);
}

void adelante(bool caso, byte PWMI, byte PWMD, int T){
    fastWrite(MOTL, HIGH);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, HIGH);
    analogWrite(PWMR, PWMD);
    espera(caso, LOW, T);
}

void atras(byte PWMI, byte PWMD, int T){
    fastWrite(MOTL, LOW);
    analogWrite(PWML,PWMI);
    fastWrite(MOTR, LOW);
    analogWrite(PWMR, PWMD);
    espera(true, LOW, T);
}

void paro(int T){
    fastWrite(MOTL, LOW);
    analogWrite(PWML, 0);
    fastWrite(MOTR, LOW);
    analogWrite(PWMR, 0);
    delay(T);
}