//

#define NUM_SF 5
#define RAN_SP 700
#define SFLL 17
#define SFL 16
#define SF 15
#define SFR 19
#define SFLR PINB, 3
#define SP_L A0
#define SP_R A4
#define MOTL PORTD, 6
#define MOTR PORTD, 3
#define PWML 4
#define PWMR 5
#define PIN_SGY_1 PIND, 2
#define PIN_SGY_2 PIND, 7
#define PIN_LED_SGY_1 PORTD, 0
#define PIN_LED_SGY_2 PORTB, 5
#define ARRANCADOR PINB, 2
#define TIME_PER_INT 0x600
#define STOP_INT TCCR1B = 0; TCNT1 = 0; flagInt = false;
#define CONT_INT TCCR1B |= (1 << CS10); TCNT1 = TIME_PER_INT;
#define CALL_CONTROL onControl = true; control(); onControl = false;
#define realFastRead(pin, numPin) (((pin) >> (numPin)) & 0x01)
#define fastRead(pin_numPin) realFastRead(pin_numPin)
#define realFastWrite(port, numPin, value) ((value) ? ((port) |= (1U << (numPin))) : ((port) &= ~(1U << (numPin))))
#define fastWrite(port_numPin, value) realFastWrite(port_numPin, value)
#define READ_ARR (fastRead(ARRANCADOR))
#define LECT_ARR(state) if(READ_ARR == state){if(!state){if((fastRead(MOTL)) && (fastRead(MOTR))) paro();} return;}
#define espera(caso, timeLapse) if(!timeLapse) return; else{\
            STOP_INT\
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
            CONT_INT\
/**//**/}

byte sens, sgy;
bool izq = false, der = false;

//VARIABLES
#define KD 30.0
#define KP 100.0
#define setPoint 30
#define MAX_SPEED 255
bool on_view = false;
byte posicion = 0;
signed char ultEntrada = 0;
volatile bool flagInt = false;
volatile bool onControl = false;

void lectura(void);
void secLeds(void);
void tracking(void);
//void (*selSgy[])(void) = {sgy1, sgy2, sgy3, sgy4};
void control(void);
int ponderacion(void);
void busqueda(void);
void izquierda(bool caso, byte PWMI, byte PWMD, int T);
void derecha(bool caso, byte PWMI, byte PWMD, int T);
void adelante(bool caso, byte PWMI, byte PWMD, int T);
void paro(void);

void setup(){
    DDRB = 0B00100000;
    DDRD = 0B01001001;
    paro();

    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 |= (1 << TOIE1);
    sens = sgy = 0;
}

void loop(){
    while(1){
        secLeds();

        if(READ_ARR){
            switch(sgy){
                case 0B00000000:
                    izquierda(true, 248, 248, 120);
                break;

                case 0B00000001:
                    derecha(true, 248, 248, 120);
                break;

                case 0B00000010:
                    izquierda(true, 248, 248, 120);
                break;

                case 0B00000011:
                    izquierda(true, 248, 248, 240);
                break;

                default:
                break;
            }
        }
    }
}

void lectura(void){
    (digitalRead(SFLL)) ? sens |= (1U << 0) : sens &= ~(1U << 0);
    (digitalRead(SFL)) ? sens |= (1U << 1) : sens &= ~(1U << 1);
    (digitalRead(SF)) ? sens |= (1U << 2) : sens &= ~(1U << 2);
    (digitalRead(SFR)) ? sens |= (1U << 3) : sens &= ~(1U << 3);
    (fastRead(SFLR)) ? sens |= (1U << 4) : sens &= ~(1U << 4);
    (analogRead(SP_L) < RAN_SP) ? sens |= (1U << 5) : sens &= ~(1U << 5);
    (analogRead(SP_R) < RAN_SP) ? sens |= (1U << 6) : sens &= ~(1U << 6);

    if((bitRead(sens, 0)) && (!bitRead(sens, 4))) izq = true;
    else if((!bitRead(sens, 0)) && (bitRead(sens, 4))) der = true;
    else if((bitRead(sens, 0)) && (bitRead(sens, 4))) izq = der = false;
}

void secLeds(void){
    unsigned int beginMillis = 0;

    do{
        (fastRead(PIN_SGY_1)) ? (sgy |= (1U << 0)) : (sgy &= ~(1U << 1));
        (fastRead(PIN_SGY_2)) ? (sgy |= (1U << 0)) : (sgy &= ~(1U << 1));
        LECT_ARR(HIGH) beginMillis = millis();

        switch(sgy){
        case 0B00000000:
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_2, HIGH);
            do{
                LECT_ARR(HIGH)
            }while((millis() - beginMillis) <= 500);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, LOW); fastWrite(PIN_LED_SGY_2, LOW);
            do{
                LECT_ARR(HIGH)
            }while((millis() - beginMillis) <= 500);
        break;

        case 0B00000001:
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_2, LOW);
            do{
                LECT_ARR(HIGH)
            }while((millis() - beginMillis) <= 400);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, LOW); fastWrite(PIN_LED_SGY_2, HIGH);
            do{
                LECT_ARR(HIGH)
            }while((millis() - beginMillis) <= 400);
        break;

        case 0B00000010:
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_2, HIGH);
            do{
                LECT_ARR(HIGH)
            }while((millis() - beginMillis) <= 300);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_2, LOW);
            do{
                LECT_ARR(HIGH)
            }while((millis() - beginMillis) <= 300);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, LOW); fastWrite(PIN_LED_SGY_2, LOW);
            do{
                LECT_ARR(HIGH)
            }while((millis() - beginMillis) <= 300);
        break;

        case 0B00000011:
            fastWrite(PIN_LED_SGY_1, HIGH); fastWrite(PIN_LED_SGY_2, HIGH);
            do{
                LECT_ARR(HIGH)
            }while((millis() - beginMillis) <= 200);
            beginMillis = millis();
            fastWrite(PIN_LED_SGY_1, LOW); fastWrite(PIN_LED_SGY_2, LOW);
            do{
                LECT_ARR(HIGH)
            }while((millis() - beginMillis) <= 200);
        break;

        default:
        break;
        }
    }while(!READ_ARR);
}

void tracking(void){
    CONT_INT
    do{
        lectura();
        if(!sens && !on_view) busqueda();
        LECT_ARR(LOW);

        if(bitRead(sens, 5) || bitRead(sens, 6)){
            atras(248, 248, 120);

            if(bitRead(sens, 5) && bitRead(sens, 6)){
                izquierda(false, 248, 248, 120);
            } else{
                if(bitRead(sens, 5)){
                    izquierda(false, 248, 248, 120);
                } else if(bitRead(sens, 6)){
                    derecha(false, 248, 248, 120);
                }
            }
        }

        if(sens & 0B10011111){CALL_CONTROL}
    }while(READ_ARR);
}

void control(void){
    static unsigned int timeControl = 0;
    if((millis() - timeControl) < 4) return;
    posicion = ponderacion();
    int salida;
    int error = setPoint - posicion;
    int dEntrada = (posicion - ultEntrada);

    salida = (int) KP * error - KD * dEntrada;

    if(salida < 0){
        adelante(true, MAX_SPEED, MAX_SPEED - salida, 0);
    } else if(!salida){
        adelante(true, MAX_SPEED, MAX_SPEED, 0);
    } else if(salida > 0){
        adelante(true, MAX_SPEED - salida, MAX_SPEED, 0);
    }

    ultEntrada = posicion;
    timeControl = millis();
}

int ponderacion(void){
    int avg, amnt;
    float amnt_det;
    int total;

    for(int i = 0; i < NUM_SF; ++i){
        if(bitRead(sens, i)){on_view = true; amnt_det += 0.1;}
        amnt += ((bitRead(sens, i) * 10) + 10);
    }

    if(!on_view) return 0;

    total = (int) (amnt * amnt_det);

    return total;
}

void busqueda(void){
    STOP_INT
    do{
    adelante(false, 30, 30, 10000);
    if(sens) return;
    }while(READ_ARR);
}

void izquierda(bool caso, byte PWMI, byte PWMD, int T){
    fastWrite(MOTL, LOW);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, HIGH);
    analogWrite(PWMR, PWMD);
    espera(caso, T);
}

void derecha(bool caso, byte PWMI, byte PWMD, int T){
    fastWrite(MOTL, HIGH);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, LOW);
    analogWrite(PWMR, PWMD);
    espera(caso, T);
}

void adelante(bool caso, byte PWMI, byte PWMD, int T){
    fastWrite(MOTL, HIGH);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, HIGH);
    analogWrite(PWMR, PWMD);
    espera(caso, T);
}

void atras(byte PWMI, byte PWMD, int T){
    fastWrite(MOTL, LOW);
    analogWrite(PWML, PWMI);
    fastWrite(MOTR, LOW);
    analogWrite(PWMR, PWMD);
    espera(true, T);
}

void paro(void){
    fastWrite(MOTL, LOW);
    analogWrite(PWML, 0);
    fastWrite(MOTR, LOW);
    analogWrite(PWMR, 0);
}

ISR(TIMER1_OVF_vect){
    if(!flagInt){
        flagInt = true;
        ultEntrada = ponderacion();
    }

    if(onControl) return;
    control();
}