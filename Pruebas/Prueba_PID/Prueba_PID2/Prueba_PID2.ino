#define MOTL 4
#define PWML 6
#define MOTR 3
#define PWMR 5

#define SENLL 19
#define SENL 18
#define SENF 17
#define SENLR 16
#define SENR 15
#define SENPL 20
#define SENPR 21
#define NUM_S 5
#define ARR 2

#define KP 12
#define KD 30
#define SETPOINT 0
#define MAX_SPEED 255
#define NO_SPEED 0

byte vSteARR = 0;
byte sens = 0;
byte on_view = 0;

void setup(){
    pinMode(MOTL, OUTPUT); pinMode(MOTR, OUTPUT);
    pinMode(SENLL, INPUT); pinMode(SENL, INPUT); pinMode(SENF, INPUT); pinMode(SENLR, INPUT); pinMode(SENR, INPUT);
    pinMode(ARR, INPUT);
    digitalWrite(MOTL, LOW); digitalWrite(MOTR, LOW);
    analogWrite(PWML, 0); analogWrite(PWMR, 0);

    Serial.begin(9600);

}

void loop(){
    if(digitalRead(ARR)) control();
    delay(4);

}

void lectura(){

    if(digitalRead(SENLL)) (sens |= (1 << 0)); else (sens &= ~(1 << 0));
    if(digitalRead(SENL)) (sens |= (1 << 1)); else (sens &= ~(1 << 1));
    if(digitalRead(SENF)) (sens |= (1 << 2)); else (sens &= ~(1 << 2));
    if(digitalRead(SENLR)) (sens |= (1 << 3)); else (sens &= ~(1 << 3));
    if(digitalRead(SENR)) (sens |= (1 << 4)); else (sens &= ~(1 << 4));

    (sens) ? (on_view = true) : (on_view = false);

}

void control(){

    static int ultEntrada = 0;
    int posicion = ponderacion();
    if(!on_view) return;
    int error = SETPOINT - posicion;
    int dError = posicion - ultEntrada;

    int salida = (error * KP);

    if(salida < 0){
        salida *= -1;
        izquierda(salida, 0, 4);
        //izquierda(salida, salida, 4);
        //adelante(salida, 0, 4);
    } else if(!salida){
        adelante(0, 0, 4);
    } else if(salida > 0){
        derecha(0, salida, 4);
        //derecha(salida, salida, 4);
        //adelante(0, salida, 4);
    }

    ultEntrada = posicion;

}

int8_t ponderacion(){

    int amnt = -20, amnt_det = 0, n_det = 0;
    lectura();
    if(!on_view) return 0;

    for(int i = 0; i < NUM_S; ++i){
        if(bitRead(sens, i)){
            amnt_det += amnt;
            ++n_det;
        }
        amnt += 10;
    }
  Serial.println(amnt_det);
  return (amnt_det / n_det);
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
