//ROBOT ORION - MACRODEV

#define MOTL
#define PWML
#define MOTR
#define PWMR
#define SFL
#define SFR
#define SPL
#define SPR
#define L_SL analogRead(SFL)
#define L_SR analogRead(SFR)
#define L_SPL analogRead(SPL)
#define L_SPR analogRead(SPR)
#define ARRANCADOR
#define LECT_ARR(state) (digitalRead(ARRANCADOR) == (state))
byte pins_sgy[2] = {};
byte p_leds_sgy[2] = {};

byte i, j;
unsigned int l;
int k;

void setup(){
    pinMode(MOTL, OUTPUT);
    pinMode(MOTR, OUTPUT);
    pinMode(ARRANCADOR, INPUT);
    pinMode(pins_sgy[0], INPUT);
    pinMode(pins_sgy[1], INPUT);
    pinMode(p_leds_sgy[0], OUTPUT);
    pinMode(p_leds_sgy[1], OUTPUT);

    i = j = l = k = 0;
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