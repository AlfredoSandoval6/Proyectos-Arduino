//ROBOT ORION - MACRODEV

#define MOTLA 4
#define MOTLB 3
#define PWML 5
#define MOTRA 7
#define MOTRB 8
#define PWMR 6
#define SFLL A0
#define SFL A1
#define SFR A2
#define SFLR A3
#define SP_L A4
#define SP_R A5
#define RAN_SF 250
#define RAN_SP 880
#define L_SFLL analogRead(SFLL)
#define L_SFL analogRead(SFL)
#define L_SFR analogRead(SFR)
#define L_SFLR analogRead(SFLR)
#define L_SP_L analogRead(SP_L)
#define L_SP_R analogRead(SP_R)
#define ARRANCADOR 2
const byte pins_sgy[2] = {5, 6};
const byte p_leds_sgy[2] = {0, 1};
byte sens, sgy;
bool izq = false, der = false;
byte i, j;
volatile byte k;
unsigned int l;

void activator();
void espera(bool caso, int timeLapse);
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
    pinMode(MOTLA, OUTPUT);
    pinMode(MOTLB, OUTPUT);
    pinMode(MOTRA, OUTPUT);
    pinMode(MOTRB, OUTPUT);
    digitalWrite(MOTLA, LOW); digitalWrite(MOTLB, LOW); analogWrite(PWML, 0);
    digitalWrite(MOTRA, LOW); digitalWrite(MOTRB, LOW); analogWrite(PWMR, 0);
    pinMode(ARRANCADOR, INPUT);
    pinMode(pins_sgy[0], INPUT);
    pinMode(pins_sgy[1], INPUT);
    pinMode(p_leds_sgy[0], OUTPUT);
    pinMode(p_leds_sgy[1], OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ARRANCADOR), activator, CHANGE);

    i = j = l = k = sens = sgy = 0;
    //delay(3000);
}

void loop(){

    secLeds();
    i = j = l = 0;

    //if(k) estrategia();

}

void activator(){
    k = !k;
}

void espera(bool caso, int timeLapse){
    int currMillis, beginMillis;

    beginMillis = millis();
    if(caso){
        do{
            currMillis = millis();
            if(!k) return;
        }while((currMillis - beginMillis) >= timeLapse);
    } else{
        do{
            currMillis = millis();
            if(!k) return;
            lectura();
            if(sens) return;
        }while((currMillis - beginMillis) >= timeLapse);
    }
}

void secLeds(){
    int currMillis = 0, beginMillis = 0, timeLapse = 0;
    byte dWLedsSgy = 0;
    // i = ps0, j = ps1
    if(digitalRead(pins_sgy[0])) sgy |= (1 << 0); else sgy &= ~(1 << 0); //HIGH == 0x1 == (int)1;
    if(digitalRead(pins_sgy[1])) sgy |= (1 << 1); else sgy &= ~(1 << 1); //LOW == 0x0 == (int)0;

    beginMillis = millis();
    //if(k) return;

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

    digitalWrite(p_leds_sgy[0], bitRead(dWLedsSgy, 0)); digitalWrite(p_leds_sgy[1], bitRead(dWLedsSgy, 1));
    do{
        currMillis = millis();
        //if(k) return;
    }while((currMillis - beginMillis) <= timeLapse);
    beginMillis = currMillis;
    digitalWrite(p_leds_sgy[0], bitRead(dWLedsSgy, 2)); digitalWrite(p_leds_sgy[1], bitRead(dWLedsSgy, 3));
    do{
        currMillis = millis();
        //if(k) return;
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
    }while(k);
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
    if(!k) return;

    if(bitRead(sens, 4) || bitRead(sens, 5)){
        if(bitRead(sens, 4) && bitRead(sens, 5)){
            atras(120); izquierda(true, 240, 240, 120);
        }
        if(bitRead(sens, 4)){
            atras(120); izquierda(true, 240, 240, 120);
        }
        if(bitRead(sens, 5)){
            atras(120); adelante(true, 240, 240, 120);
        }
        if(!k) return;
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

    if((!k) || (sens)) return;
    do{
        adelante(false, 35, 35, 2);
        if(!k) return;
    }while(sens == 0);
}

void izquierda(bool caso, byte PWMI, byte PWMD, int T){
    digitalWrite(MOTLA, HIGH);
    digitalWrite(MOTLB, LOW);
    analogWrite(PWML, PWMI);
    digitalWrite(MOTRA, LOW);
    digitalWrite(MOTRB, HIGH);
    analogWrite(PWMR, PWMD);
    espera(caso, T);
}

void derecha(bool caso, byte PWMI, byte PWMD, int T){
    digitalWrite(MOTLA, LOW);
    digitalWrite(MOTLB, HIGH);
    analogWrite(PWML, PWMI);
    digitalWrite(MOTRA, HIGH);
    digitalWrite(MOTRB, LOW);
    analogWrite(PWMR, PWMD);
    espera(caso, T);
}

void adelante(bool caso, byte PWMI, byte PWMD, int T){
    digitalWrite(MOTLA, LOW);
    digitalWrite(MOTLB, HIGH);
    analogWrite(PWML, PWMI);
    digitalWrite(MOTRA, LOW);
    digitalWrite(MOTRB, HIGH);
    analogWrite(PWMR, PWMD);
    espera(caso, T);
}

void atras(int T){
    digitalWrite(MOTLA, HIGH);
    digitalWrite(MOTLB, LOW);
    analogWrite(PWML, 255);
    digitalWrite(MOTRA, HIGH);
    digitalWrite(MOTRB, LOW);
    analogWrite(PWMR, 255);
    espera(true, T);
}

void paro(int T){
    digitalWrite(MOTLA, LOW);
    digitalWrite(MOTLB, LOW);
    analogWrite(PWML, 0);
    digitalWrite(MOTRA, LOW);
    digitalWrite(MOTRB, LOW);
    analogWrite(PWMR, 0);
    espera(true, T);
}
