#include <Servo.h>

Servo servoMotor1;

#define MOTL 4
#define PWML 6
#define MOTR 3
#define PWMR 5

#define SENLL 19
#define SENDL 9
#define SENL 18
#define SENF 17
#define SENR 16
#define SENDR 11
#define SENLR 15
#define SENPL A6
#define SENPR A7
#define NUM_S 7
#define ARR 2
#define PSGY1 8

#define KP 6
#define KD 3
#define SETPOINT 40
#define MAX_SPEED 250
#define NO_SPEED 0

unsigned int sens = 0;
byte ste = false;
volatile byte vSteARR = 0;

void sgy1();
void sgy2();
void lectura();
void posAnalisis();
void bus_leo();
void control();
int ponderacion();
void izquierda(byte PWMI, byte PWMD, int T);
void derecha(byte PWMI, byte PWMD, int T);
void adelante(byte PWMI, byte PWMD, int T);
void atras(byte PWMI, byte PWMD, int T);
void paro(int T);
void (*SGTY[])() = {sgy1, sgy2};

void setup(){

    pinMode(MOTL, OUTPUT); pinMode(MOTR, OUTPUT);
    pinMode(SENLL, INPUT); pinMode(SENL, INPUT); pinMode(SENF, INPUT); pinMode(SENLR, INPUT); pinMode(SENR, INPUT);
    pinMode(ARR, INPUT); pinMode(PSGY1, INPUT);
    digitalWrite(MOTL, LOW); digitalWrite(MOTR, LOW);
    analogWrite(PWML, 0); analogWrite(PWMR, 0);
    attachInterrupt(digitalPinToInterrupt(ARR), cSteARR, CHANGE);
    //Serial.begin(9600);
    servoMotor1.attach(10);

}

void loop(){
    if(!ste){
        paro(0);
        servoMotor1.write(0);
        ste = true;
    }
    if(!vSteARR) paro(0);
    if(vSteARR){
        servoMotor1.write(90);
        ste = false;
        selSgy();
    }
}

void cSteARR(){

    vSteARR = !vSteARR;

}

void lectura(){

    (digitalRead(SENLL)) ? (sens |= (1 << 0)) : (sens &= ~(1 << 0));
    (digitalRead(SENDL)) ? (sens |= (1 << 1)) : (sens &= ~(1 << 1));
    (digitalRead(SENL)) ? (sens |= (1 << 2)) : (sens &= ~(1 << 2));
    (digitalRead(SENF)) ? (sens |= (1 << 3)) : (sens &= ~(1 << 3));
    (digitalRead(SENR)) ? (sens |= (1 << 4)) : (sens &= ~(1 << 4));
    (digitalRead(SENDR)) ? (sens |= (1 << 5)) : (sens &= ~(1 << 5));
    (digitalRead(SENLR)) ? (sens |= (1 << 6)) : (sens &= ~(1 << 6));
    (analogRead(SENPL) <= 400) ? (sens |= (1 << 7)) : (sens &= ~(1 << 7));
    (analogRead(SENPR) <= 400) ? (sens |= (1 << 8)) : (sens &= ~(1 << 8));

}

void selSgy(){

    byte sgy = 0;

    (digitalRead(PSGY1)) ? (sgy |= (1 << 0)) : (sgy &= ~(1 << 0));

    (*SGTY[sgy])();

    while(vSteARR) posAnalisis();

}

void sgy1(){

}

void sgy2(){

    unsigned long taim = millis();

    adelante(87, 50, 0);
    do{
        lectura();
        if(sens || (!vSteARR)) return;
    }while((millis() - taim) <= 2000);

}

void posAnalisis(){

    lectura();
    if(!sens) bus_leo();

    if(bitRead(sens, 7) || bitRead(sens, 8)){
        if(bitRead(sens, 7) && bitRead(sens, 8)){
            atras(MAX_SPEED, MAX_SPEED, 100);
            izquierda(240, 240, 125);
        } else if(bitRead(sens, 7)){
            atras(MAX_SPEED, MAX_SPEED, 100);
            izquierda(240, 240, 125);
        } else if(bitRead(sens, 8)){
            atras(MAX_SPEED, MAX_SPEED, 100);
            derecha(240, 240, 125);
        }
    } else if(sens & 0B0000000001111111) control();

}

void bus_leo(){

    unsigned long taim = millis();

    adelante(40, 40, 0);
    do{

        lectura();
        if(sens || (!vSteARR)) return;

    }while((millis() - taim) <= 100);

    taim = millis();
    paro(0);
    do{

        lectura();
        if(sens || (!vSteARR)) return;

    }while((millis() - taim) <= 3000);

}

void control(){

    static int ultEntrada = 0;
    int posicion = ponderacion();
    if((!sens) || (!vSteARR)) return;
    int error = SETPOINT - posicion;
    int dEntrada = posicion - ultEntrada;

    int salida = (error * KP) - (dEntrada * KD);

    if(salida < 0){
        salida *= -1;
        //izquierda(salida, 0, 2);
        izquierda(salida, salida, 2);
        //adelante(salida, 0, 2);
    } else if(!salida){
        adelante(MAX_SPEED, MAX_SPEED, 2);
    } else if(salida > 0){
        //derecha(0, salida, 2);
        derecha(salida, salida, 2);
        //adelante(0, salida, 2);
    }

    ultEntrada = posicion;

}

int ponderacion(){

    int amnt = 10, amnt_det = 0, n_det = 0;
    lectura();
    //Serial.println(sens);
    if(!sens){
        paro(0);
        return 0;
    }

    for(byte i = 0; i < NUM_S; ++i){
        if(bitRead(sens, i)){
            amnt_det += amnt;
            ++n_det;
        }
        amnt += 10;
    }
    amnt_det /= n_det;

    return amnt_det;

}

void izquierda(byte PWMI, byte PWMD, int T){

    digitalWrite(MOTL, LOW);
    digitalWrite(MOTR, HIGH);
    analogWrite(PWML, PWMI);
    analogWrite(PWMR, PWMD);
    if(!T) return;
    unsigned long taim = millis();
    do{
        if(!vSteARR){paro(0); return;}
    }while((millis() - taim) <= T);

}

void derecha(byte PWMI, byte PWMD, int T){

    digitalWrite(MOTL, HIGH);
    digitalWrite(MOTR, LOW);
    analogWrite(PWML, PWMI);
    analogWrite(PWMR, PWMD);
    if(!T) return;
    unsigned long taim = millis();
    do{
        if(!vSteARR){paro(0); return;}
    }while((millis() - taim) <= T);

}

void adelante(byte PWMI, byte PWMD, int T){

    digitalWrite(MOTL, HIGH);
    digitalWrite(MOTR, HIGH);
    analogWrite(PWML, PWMI);
    analogWrite(PWMR, PWMD);
    if(!T) return;
    unsigned long taim = millis();
    do{
        if(!vSteARR){paro(0); return;}
    }while((millis() - taim) <= T);

}

void atras(byte PWMI, byte PWMD, int T){

    digitalWrite(MOTL, LOW);
    digitalWrite(MOTR, LOW);
    analogWrite(PWML, PWMI);
    analogWrite(PWMR, PWMD);
    if(!T) return;
    unsigned long taim = millis();
    do{
        if(!vSteARR){paro(0); return;}
    }while((millis() - taim) <= T);

}

void paro(int T){

    digitalWrite(MOTL, LOW);
    digitalWrite(MOTR, LOW);
    analogWrite(PWML, 0);
    analogWrite(PWMR, 0);
    if(!T) return;
    delay(T);

}
