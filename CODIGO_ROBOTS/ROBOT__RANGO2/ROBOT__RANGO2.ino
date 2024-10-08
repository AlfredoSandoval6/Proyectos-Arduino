#define MOTL 4
#define PWML 6
#define MOTR 3
#define PWMR 5

#define SENLL 19
#define SENL 18
#define SENF 17
#define SENR 16
#define SENLR 15
#define SENPL A7
#define SENPR A6
#define NUM_S 5
#define ARR 2
#define PSGY1 8
#define PSGY2 7

#define KP 12
#define KD 10
#define SETPOINT 30
#define MAX_SPEED 250
#define NO_SPEED 0

byte vSteARR = 0;
byte sens = 0;
byte ste = 0;

void sgy1();
void sgy2();
void sgy3();
void sgy4();
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
void (*SGTY[])() = {sgy1, sgy2, sgy3, sgy4};

void setup(){

    pinMode(MOTL, OUTPUT); pinMode(MOTR, OUTPUT);
    pinMode(SENLL, INPUT); pinMode(SENL, INPUT); pinMode(SENF, INPUT); pinMode(SENLR, INPUT); pinMode(SENR, INPUT);
    pinMode(ARR, INPUT);
    digitalWrite(MOTL, LOW); digitalWrite(MOTR, LOW);
    analogWrite(PWML, 0); analogWrite(PWMR, 0);
    //Serial.begin(9600);

}

void loop(){

    if(digitalRead(ARR)) selSgy();

    if(!digitalRead(ARR)){
        paro(0);
    }

}

void lectura(){

    (digitalRead(SENLL)) ? (sens |= (1 << 0)) : (sens &= ~(1 << 0));
    (digitalRead(SENL)) ? (sens |= (1 << 1)) : (sens &= ~(1 << 1));
    (digitalRead(SENF)) ? (sens |= (1 << 2)) : (sens &= ~(1 << 2));
    (digitalRead(SENR)) ? (sens |= (1 << 3)) : (sens &= ~(1 << 3));
    (digitalRead(SENLR)) ? (sens |= (1 << 4)) : (sens &= ~(1 << 4));
    if(analogRead(SENPL) <= 100) sens |= (1 << 5); else sens &= ~(1 << 5);
    if(analogRead(SENPR) <= 100) sens |= (1 << 6); else sens &= ~(1 << 6);

}

void selSgy(){

    byte sgy = 0;

    (digitalRead(PSGY1)) ? (sgy |= (1 << 0)) : (sgy &= ~(1 << 0));
    (digitalRead(PSGY2)) ? (sgy |= (1 << 1)) : (sgy &= ~(1 << 1));

    (*SGTY[sgy])();

    while(digitalRead(ARR)) posAnalisis();

}


void sgy1(){

}

void sgy2(){

    unsigned long taim = millis();

    adelante(87, 50, 0);
    do{
        lectura();
        if(sens || (!digitalRead(ARR))) return;
    }while((millis() - taim) <= 500);

}

void sgy3(){

    unsigned long taim = millis();

    adelante(50, 87, 0);
    do{
        lectura();
        if(sens || (!digitalRead(ARR))) return;
    }while((millis() - taim) <= 500);

}

void sgy4(){

    unsigned long taim = millis();

    adelante(25, 25, 0);
    do{
        lectura();
        if(sens || (!digitalRead(ARR))) return;
    }while((millis() - taim) <= 500);
    
}

void posAnalisis(){

    lectura();
    if(!sens) bus_leo();

    if(bitRead(sens, 5) || bitRead(sens, 6)){
        if(bitRead(sens, 5) && bitRead(sens, 6)){
            atras(MAX_SPEED, MAX_SPEED, 75);
            izquierda(240, 240, 100);
        } else{
            if(bitRead(sens, 5)){
                atras(MAX_SPEED, MAX_SPEED, 75);
                izquierda(240, 240, 75);
            } else if(bitRead(sens, 6)){
                atras(MAX_SPEED, MAX_SPEED, 75);
                derecha(240, 240, 75);
            }
        }
        
    } else if(sens & 0B00011111) control();

}

void bus_leo(){

    unsigned long taim = millis();

    adelante(150, 150, 0);
    do{
        lectura();
        if(sens || (!digitalRead(ARR))) return;
    }while((millis() - taim) <= 100);

    taim = millis();
    paro(0);
    do{
        lectura();
        if(sens || (!digitalRead(ARR))) return;
    }while((millis() - taim) <= 1000);

}

void control(){
    //Serial.println(F("CONTROL"));
    static int ultEntrada;
    int posicion = ponderacion();
    if(!sens || (!digitalRead(ARR))) return;
    int error = SETPOINT - posicion;
    int dEntrada = posicion - ultEntrada;

    int salida = (error * KP) - (dEntrada * KD);

    if(salida > MAX_SPEED) salida = MAX_SPEED;
    else if(salida < -MAX_SPEED) salida = -MAX_SPEED;
    //Serial.println(salida);
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
    //Serial.print(amnt_det);
    //Serial.print("\t");
  return amnt_det;
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
