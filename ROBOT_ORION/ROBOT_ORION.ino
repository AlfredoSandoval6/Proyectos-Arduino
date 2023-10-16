//ROBOT ORION - MACRODEV

#define MOTL
#define PWML
#define MOTR
#define PWMR
#define SENL
#define SENR
#define SPL
#define SPR
#define L_SL analogRead(SENL)
#define _SR analogRead(SENR)
#define L_SPL analogRead(SPL)
#define L_SPR analogRead(SPR)
#define ARRANCADOR
#define LECT_ARR(state) (digitalRead(ARRANCADOR) == state)

void setup(){
    pinMode(MOTL, OUTPUT);
    pinMode(MOTR, OUTPUT);
}

void loop(){
    
}

void izquierda(byte PWMI, byte PWMD, int T){
    digitalWrite(MOTL, LOW);
    analogWrite(PWML, PWMI);
    digitalWrite(MOTR, HIGH);
    analogWrite(PWMR, PWMD);
    delay(T);
}

void derecha(byte PWMI, byte PWMD, int T){
    digitalWrite(MOTL, HIGH);
    analogWrite(PWML, PWMI);
    digitalWrite(MOTR, LOW);
    analogWrite(PWMR, PWMD);
    delay(T);
}

void adelante(byte PWMI, byte PWMD, int T){
    digitalWrite(MOTL, HIGH);
    analogWrite(PWML, PWMI);
    digitalWrite(MOTR, HIGH);
    analogWrite(PWMR, PWMD);
    delay(T);
}

void atras(int T){
    digitalWrite(MOTL, LOW);
    analogWrite(PWML, 255);
    digitalWrite(MOTR, LOW);
    analogWrite(PWMR, 255);
    delay(T);
}

void paro(int T){
    digitalWrite(MOTL, LOW);
    analogWrite(PWML, 0);
    digitalWrite(MOTR, LOW);
    analogWrite(PWMR, 0);
    delay(T);
}