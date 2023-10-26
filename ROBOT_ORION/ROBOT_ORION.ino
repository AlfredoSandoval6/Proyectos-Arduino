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
#define SPL A4
#define SPR A5
#define L_SFLL analogRead(SFLL)
#define L_SFL analogRead(SFL)
#define L_SFR analogRead(SFR)
#define L_SFLR analogRead(SFLR)
#define L_SPL analogRead(SPL)
#define L_SPR analogRead(SPR)
#define ARRANCADOR 2
byte pins_sgy[2] = {};
byte p_leds_sgy[2] = {0, 1};
byte sens, sgy;
bool izq = false, der = false;
byte i, j;
volatile byte k;
unsigned int l;

void activator();
void secLeds();
void estrategia();
void lectura();
void analisisPos();
void busqueda();
void izquierda(byte PWMI, byte PWMD, int T);
void derecha(byte PWMI, byte PWMD, int T);
void adelante(byte PWMI, byte PWMD, int T);
void atras(int T);
void paro(int T);

void setup(){
    pinMode(MOTLA, OUTPUT);
    pinMode(MOTLB, OUTPUT);
    pinMode(MOTRA, OUTPUT);
    pinMode(MOTRB, OUTPUT);
    pinMode(ARRANCADOR, INPUT);
    pinMode(pins_sgy[0], INPUT);
    pinMode(pins_sgy[1], INPUT);
    pinMode(p_leds_sgy[0], OUTPUT);
    pinMode(p_leds_sgy[1], OUTPUT);
    
    attachInterrupt(digitalPinToInterrupt(ARRANCADOR), activator, CHANGE);

    i = j = l = k = sens = sgy = 0;
    delay(3000);
}

void loop(){

    secLeds();
    i = j = l = 0;

    if(k) estrategia();

}

void activator(){
    k = !k;
}

void secLeds(){
    int currMillis = 0, prevMillis = 0, timeLapse = 0;
    // i = ps0, j = ps1
    if (digitalRead(pins_sgy[0])) sgy |= (1 << 0); else sgy &= ~(1 << 0); //HIGH == 0x1 == (int)1;
    if (digitalRead(pins_sgy[1])) sgy |= (1 << 1); else sgy &= ~(1 << 1); //LOW == 0x0 == (int)0;

    switch(sgy){
        case 0:
            timeLapse = 500;
            digitalWrite(p_leds_sgy[0], HIGH); digitalWrite(p_leds_sgy[1], HIGH);
            do{
                currMillis = millis();
                if(k) return;
            }while((currMillis - prevMillis) >= timeLapse);
            digitalWrite(p_leds_sgy[0], LOW); digitalWrite(p_leds_sgy[1], LOW);
        break;

        case 1:
            timeLapse = 400;
            digitalWrite(p_leds_sgy[0], HIGH); digitalWrite(p_leds_sgy[1], LOW);
            do{
                currMillis = millis();
                if(k) return;
            }while((currMillis - prevMillis) >= timeLapse);
            digitalWrite(p_leds_sgy[0], HIGH); digitalWrite(p_leds_sgy[1], LOW);
        break;

        case 2:
            timeLapse = 300;
            digitalWrite(p_leds_sgy[0], HIGH); digitalWrite(p_leds_sgy[1], HIGH);
            do{
                currMillis = millis();
                if(k) return;
            }while((currMillis - prevMillis) >= timeLapse);
            digitalWrite(p_leds_sgy[0], HIGH); digitalWrite(p_leds_sgy[1], LOW);
        break;

        case 3:
            timeLapse = 200;
            digitalWrite(p_leds_sgy[0], HIGH); digitalWrite(p_leds_sgy[1], HIGH);
            do{
                currMillis = millis();
                if(k) return;
            }while((currMillis - prevMillis) >= timeLapse);
            digitalWrite(p_leds_sgy[0], LOW); digitalWrite(p_leds_sgy[1], LOW);
        break;

        default:
        break;
    }
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

    analisisPos();
}

void lectura(){
    if(L_SFLL == HIGH){sens |= (1 << 0); izq = true;}else{sens &= ~(1 << 0);}
    if(L_SFL == HIGH){sens |= (1 << 1);}else{sens &= ~(1 << 1);}
    if(L_SFR == HIGH){sens |= (1 << 2);}else{sens &= ~(1 << 2);}
    if(L_SFLR == HIGH){sens |= (1 << 3); der = true;}else{sens &= ~(1 << 3);}
    if(L_SPL == HIGH){sens |= (1 << 4);}else{sens &= ~(1 << 4);}
    if(L_SPR == HIGH){sens |= (1 << 5);}else{sens &= ~(1 << 5);}
}

void analisisPos(){
    lectura();
    if(sens == 0) busqueda();
    if(k) return;

    if(bitRead(sens, 4) || bitRead(sens, 5)){
        if(bitRead(sens, 4) && bitRead(sens, 5)){
            atras(120); izquierda(240, 240, 120);
        }
        if(bitRead(sens, 4)){
            atras(120); izquierda(240, 240, 120);
        }
        if(bitRead(sens, 5)){
            atras(120); adelante(240, 240, 120);
        }
    } else{
        if(bitRead(sens, 0)){
            if(bitRead(sens, 1)){
                if(bitRead(sens, 2)){
                    if(bitRead(sens, 3)){

                    }
                }
            }
        }


    }
}

void busqueda(){
    do{
        adelante(35, 35, 2);
        lectura();
        if(k) return;
    }while(sens == 0);
}

void izquierda(byte PWMI, byte PWMD, int T){
    digitalWrite(MOTLA, HIGH);
    digitalWrite(MOTLB, LOW);
    analogWrite(PWML, PWMI);
    digitalWrite(MOTRA, LOW);
    digitalWrite(MOTRB, HIGH);
    analogWrite(PWMR, PWMD);
    delay(T);
}

void derecha(byte PWMI, byte PWMD, int T){
    digitalWrite(MOTLA, LOW);
    digitalWrite(MOTLB, HIGH);
    analogWrite(PWML, PWMI);
    digitalWrite(MOTRA, HIGH);
    digitalWrite(MOTRB, LOW);
    analogWrite(PWMR, PWMD);
    delay(T);
}

void adelante(byte PWMI, byte PWMD, int T){
    digitalWrite(MOTLA, LOW);
    digitalWrite(MOTLB, HIGH);
    analogWrite(PWML, PWMI);
    digitalWrite(MOTRA, LOW);
    digitalWrite(MOTRB, HIGH);
    analogWrite(PWMR, PWMD);
    delay(T);
}

void atras(int T){
    digitalWrite(MOTLA, HIGH);
    digitalWrite(MOTLB, LOW);
    analogWrite(PWML, 255);
    digitalWrite(MOTRA, HIGH);
    digitalWrite(MOTRB, LOW);
    analogWrite(PWMR, 255);
    delay(T);
}

void paro(int T){
    digitalWrite(MOTLA, LOW);
    digitalWrite(MOTLB, LOW);
    analogWrite(PWML, 0);
    digitalWrite(MOTRA, LOW);
    digitalWrite(MOTRB, LOW);
    analogWrite(PWMR, 0);
    delay(T);
}