#define MOTL 4
#define PWML 6
#define MOTR 3
#define PWMR 5

#define SENLL 19
#define SENL 18
#define SENF 17
#define SENLR 15
#define SENR 16
#define SENPL 0
#define SENPR 0
#define NUM_S 5
#define ARR 2
#define PSGY1 0
#define PSGY2 0

#define KP 5
#define KD 30
#define SETPOINT 0
#define MAX_SPEED 255
#define NO_SPEED

byte vSteARR = 0;
byte sens = 0;
bool on_view = 0;
void (*sgys[4])(void) = {sgy1, sgy2, sgy3, sgy4};

void setup(){
    pinMode(MOTL, OUTPUT); pinMode(MOTR, OUTPUT);
    pinMode(SENLL, INPUT); pinMode(SENL, INPUT); pinMode(SENF, INPUT); pinMode(SENLR, INPUT); pinMode(SENR, INPUT);
    pinMode(ARR, INPUT);

    attachInterrupt(digitalPinToInterrupt(ARR), steARR, CHANGE);
}

void steARR();
void lectura();
void control();
int ponderacion();
void izquierda(byte PWMI, byte PWMD, int T);
void derecha(byte PWMI, byte PWMD, int T);
void adelante(byte PWMI, byte PWMD, int T);
void atras(byte PWMI, byte PWMD, int T);
void paro(int T);

void loop(){

    if(vSteARR) posAnalisis();

}

void steARR(){

    vSteARR = !vSteARR;

}

void lectura(){

    digitalRead(SENLL) ? sens |= (1 << 0) : sens &= ~(1 << 0);
    digitalRead(SENL) ? sens |= (1 << 1) : sens &= ~(1 << 1);
    digitalRead(SENF) ? sens |= (1 << 2) : sens &= ~(1 << 2);
    digitalRead(SENLR) ? sens |= (1 << 3) : sens &= ~(1 << 3);
    digitalRead(SENR) ? sens |= (1 << 4) : sens &= ~(1 << 4);
    digitalRead(SENPL) ? sens |= (1 << 5) : sens &= ~(1 << 5);
    digitalRead(SENPR) ? sens |= (1 << 6) : sens &= ~(1 << 6);

    sens ? on_view = true : on_view = false;

}

void estrategia(){
    byte sgy = 0;
    digitalRead(PSGY1) ? sgy |= (1 << 0) : sgy &= ~(1 << 0);
    digitalRead(PSGY2) ? sgy |= (1 << 1) : sgy &= ~(1 << 1);

    (*sgys[sgy])();

}

void sgy1(){

}

void sgy2(){

}

void sgy3(){

}

void sgy4(){

}

void posAnalisis(){

    lectura();

    if(!sens) busqueda();

    if(bitRead(sens, 5) || bitRead(sens, 6)){
        if(bitRead(sens, 5) && (~(bitRead(sens, 6)))){
            atras(150, 150, 60);
            izquierda(MAX_SPEED, MAX_SPEED, 60);
        }
        else if((~bitRead(sens, 5)) && (bitRead(sens, 6))){
            atras(150, 150, 60);
            derecha(MAX_SPEED, MAX_SPEED, 60);
        }
        else if(bitRead(sens, 5) && bitRead(sens, 6)){
            atras(150, 150, 60);
            izquierda(MAX_SPEED, MAX_SPEED, 120);
        }
    }

    if(sens) control();

}

void busqueda(){

    do{
        adelante(30, 30, 2); lectura();
    }while(on_view);

}

void control(){

    static unsigned int timeControl = 0;
    if((millis() - timeControl) < 4) return;
    int salida = 0;
    int ultEntrada;
    int posicion = ponderacion();
    int error = SETPOINT - posicion;
    int dError = posicion - ultEntrada;

    int salida = (error * KP) - (dError * KD);

    if(salida < 0){
        adelante(MAX_SPEED - salida, MAX_SPEED, 4);
    } else if(!salida){
        adelante(MAX_SPEED, MAX_SPEED, 4);
    } else if(salida > 0){
        adelante(MAX_SPEED, MAX_SPEED - salida, 4);
    }

    ultEntrada = posicion;
    timeControl = millis();
}

int ponderacion(){

    int amnt = -20, amnt_det;

    if(!on_view) return 0;

    for(byte i = 0; i < NUM_S; ++i){
        if((bitRead(sens, i))) amnt_det += amnt;
        amnt += 10;
    }

}

void izquierda(byte PWMI, byte PWMD, int T){

    digitalWrite(MOTL, LOW);
    digitalWrite(MOTR, HIGH);
    analogWrite(PWML, PWMI);
    analogWrite(PWMR, PWMD);
    delay(T);

}

void izquierda(byte PWMI, byte PWMD, int T){

    digitalWrite(MOTL, LOW);
    digitalWrite(MOTR, HIGH);
    analogWrite(PWML, PWMI);
    analogWrite(PWMR, PWMD);
    delay(T);

}

void derecha(byte PWMI, byte PWMD, int T){

    digitalWrite(MOTL, HIGH);
    digitalWrite(MOTR, LOW);
    analogWrite(PWML, PWMI);
    analogWrite(PWMR, PWMD);
    delay(T);

}

void adelante(byte PWMI, byte PWMD, int T){

    digitalWrite(MOTL, HIGH);
    digitalWrite(MOTR, HIGH);
    analogWrite(PWML, PWMI);
    analogWrite(PWMR, PWMD);
    delay(T);

}

void atras(byte PWMI, byte PWMD, int T){

    digitalWrite(MOTL, LOW);
    digitalWrite(MOTR, LOW);
    analogWrite(PWML, PWMI);
    analogWrite(PWMR, PWMD);
    delay(T);

}

void paro(int T){

    digitalWrite(MOTL, LOW);
    digitalWrite(MOTR, LOW);
    analogWrite(PWML, 0);
    analogWrite(PWMR, 0);
    delay(T);

}