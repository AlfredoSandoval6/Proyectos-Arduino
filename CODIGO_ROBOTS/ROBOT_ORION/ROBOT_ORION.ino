//ROBOT ORION - MACRODEV

#define STBY 2
#define MOTLA 4
#define MOTLB 3
#define PWML 5
#define MOTRA 7
#define MOTRB 8
#define PWMR 6
#define ARRANCADOR 1
#define PINS_SGY_1 2
#define PINS_SGY_2 3
#define P_LEDS_SGY1 0
#define P_LEDS_SGY2 1
#define SFLL A5
#define SFL A3
#define SFR A2
#define SFLR A0
#define SP_L A4
#define SP_R A1
#define RAN_SF 250
#define RAN_SP 60
#define L_SFLL analogRead(SFLL)
#define L_SFL analogRead(SFL)
#define L_SFR analogRead(SFR)
#define L_SFLR analogRead(SFLR)
#define L_SP_L analogRead(SP_L)
#define L_SP_R analogRead(SP_R)
#define fastRead(pin, numPin) (((pin) >> (numPin)) & 0x01)
#define fastWrite(port, numPin, val) ((val) ? bitSet(port, numPin) : bitClear(port, numPin))
#define LECT_ARR(state) (fastRead(PORTB, ARRANCADOR) == state)
#define ACT_ARR(state) if(LECT_ARR(state)){fastWrite(PIND, STBY, state); if(!state){paro(1);} return;}
byte sens, sgy;
bool izq = false, der = false;
byte i, j;
volatile byte k;
unsigned int l;

void secLeds();
void estrategia();
void lectura();
void analisisPos();
void busqueda();
void izquierda(bool caso, byte PWMI, byte PWMD, int T);
void derecha(bool caso, byte PWMI, byte PWMD, int T);
void adelante(bool caso, byte PWMI, byte PWMD, int T);
void atras(int T);
void paro(int T);

void setup(){

    DDRB = 0B00000001;
    DDRD = 0B10011111;

    i = j = l = k = sens = sgy = 0;
    delay(3000);
}

void loop(){

    secLeds();
    i = j = l = 0;

    if(LECT_ARR(HIGH)) estrategia();

}

void espera(bool caso, int timeLapse){
    int currMillis, beginMillis;

    beginMillis = millis();
    if(caso){
        do{
            currMillis = millis();
            ACT_ARR(LOW);
        }while((currMillis - beginMillis) >= timeLapse);
    } else{
        do{
            currMillis = millis();
            ACT_ARR(LOW);
            lectura();
            if(sens) return;
        }while((currMillis - beginMillis) >= timeLapse);
    }
}

void secLeds(){

    int currMillis = 0, beginMillis = 0, timeLapse = 0;
    byte dWLedsSgy = 0;
    // i = ps0, j = ps1
    if(fastRead(PINB, PINS_SGY_1)) sgy |= (1 << 0); else sgy &= ~(1 << 0); //HIGH == 0x1 == (int)1;
    if(fastRead(PINB, PINS_SGY_2)) sgy |= (1 << 1); else sgy &= ~(1 << 1); //LOW == 0x0 == (int)0;

    beginMillis = millis();
    ACT_ARR(HIGH);

    switch(sgy){
        case 0:
            timeLapse = 500; dWLedsSgy = 0B00000011;
        break;

        case 1:
            timeLapse = 400; dWLedsSgy = 0B00001001;
        break;

        case 2:
            timeLapse = 300; dWLedsSgy = 0B00000111;
        break;

         case 3:
            timeLapse = 200; dWLedsSgy = 0B00000011;
        break;

        default:
        break;
    }

    fastWrite(PORTD, P_LEDS_SGY1, bitRead(dWLedsSgy, 0)); fastWrite(PORTD, P_LEDS_SGY2, bitRead(dWLedsSgy, 1));
    do{
        currMillis = millis();
        ACT_ARR(HIGH);
        if((fastRead(PINB, PINS_SGY_1) != bitRead(sgy, 0)) || (fastRead(PINB, PINS_SGY_2) != bitRead(sgy, 1))) return;
    }while((currMillis - beginMillis) <= timeLapse);
    beginMillis = currMillis;
    fastWrite(PORTD, P_LEDS_SGY1, bitRead(dWLedsSgy, 2)); fastWrite(PORTD, P_LEDS_SGY2, bitRead(dWLedsSgy, 3));
    do{
        currMillis = millis();
        ACT_ARR(HIGH);
        if((fastRead(PINB, PINS_SGY_1) != bitRead(sgy, 0)) || (fastRead(PINB, PINS_SGY_2) != bitRead(sgy, 1))) return;
    }while((currMillis - beginMillis) <= timeLapse);
}

void estrategia(){
    switch(sgy){
        case 0B00000000:
        break;

        case 0B00000001:
        break;

        case 0B00000010:        
        break;

        case 0B00000011:        
        break;

        default:
        break;
    }

    do{
        analisisPos();
    }while(LECT_ARR(HIGH));
}

void lectura(){
    if(L_SFLL > RAN_SF) sens |= (1 << 0); else sens &= ~(1 << 0);
    if(L_SFL > RAN_SF) sens |= (1 << 1); else sens &= ~(1 << 1);
    if(L_SFR > RAN_SF) sens |= (1 << 2); else sens &= ~(1 << 2);
    if(L_SFLR > RAN_SF) sens |= (1 << 3); else sens &= ~(1 << 3);
    if(L_SP_L < RAN_SP) sens |= (1 << 4); else sens &= ~(1 << 4);
    if(L_SP_R < RAN_SP) sens |= (1 << 5); else sens &= ~(1 << 5);

    if(bitRead(sens, 0) && bitRead(sens, 3)) izq = der = false;
    if(bitRead(sens, 0) && !bitRead(sens, 3)) izq = true;
    if(!bitRead(sens, 0) && bitRead(sens, 3)) der = true;
}

void analisisPos(){
    lectura();
    if(sens == 0) busqueda();
    ACT_ARR(LOW);

    if(bitRead(sens, 4) || bitRead(sens, 5)){
        if(bitRead(sens, 4) && bitRead(sens, 5)){
            atras(500); izquierda(true, 240, 240, 500);
        }
        if(bitRead(sens, 4)){
            atras(500); izquierda(true, 240, 240, 500);
        }
        if(bitRead(sens, 5)){
            atras(500); adelante(true, 240, 240, 500);
        }
        ACT_ARR(LOW);
    } else{/*

        switch(sgy){
            case 0:
            break;

            case 1:
            break;

            case 2:
            break;

            case 3:
            break;

            default:
            break;
        }
*/
    }
}

void busqueda(){
    if(izq){ izquierda(false, 240, 240, 120); izq = false;}
    if(der){ izquierda(false, 240, 240, 120); der = false;}

    if((LECT_ARR(LOW)) || (sens)) return;
    do{
        adelante(false, 35, 35, 50);
        ACT_ARR(LOW);
    }while(sens == 0);
}

void izquierda(bool caso, byte PWMI, byte PWMD, int T){
    fastWrite(PORTD, MOTLA, HIGH);
    fastWrite(PORTD, MOTLB, LOW);
    analogWrite(PWML, PWMI);
    fastWrite(PORTD, MOTRA, LOW);
    fastWrite(PORTB, MOTRB, HIGH);
    analogWrite(PWMR, PWMD);
    espera(caso, T);
}

void derecha(bool caso, byte PWMI, byte PWMD, int T){
    fastWrite(PORTD, MOTLA, HIGH);
    fastWrite(PORTD, MOTLB, LOW);
    analogWrite(PWML, PWMI);
    fastWrite(PORTD, MOTRA, HIGH);
    fastWrite(PORTB, MOTRB, LOW);
    analogWrite(PWMR, PWMD);
    espera(caso, T);
}

void adelante(bool caso, byte PWMI, byte PWMD, int T){
    fastWrite(PORTD, MOTLA, LOW);
    fastWrite(PORTD, MOTLB, HIGH);
    analogWrite(PWML, PWMI);
    fastWrite(PORTD, MOTRA, LOW);
    fastWrite(PORTB, MOTRB, HIGH);
    analogWrite(PWMR, PWMD);
    espera(caso, T);
}

void atras(int T){
    fastWrite(PORTD, MOTLA, HIGH);
    fastWrite(PORTD, MOTLB, LOW);
    analogWrite(PWML, 255);
    fastWrite(PORTD, MOTRA, HIGH);
    fastWrite(PORTB, MOTRB, LOW);
    analogWrite(PWMR, 255);
    espera(true, T);
}

void paro(int T){
    fastWrite(PORTD, MOTLA, LOW);
    fastWrite(PORTD, MOTLB, LOW);
    analogWrite(PWML, 0);
    fastWrite(PORTD, MOTRA, LOW);
    fastWrite(PORTB, MOTRB, LOW);
    analogWrite(PWMR, 0);
    espera(true, T);
}
