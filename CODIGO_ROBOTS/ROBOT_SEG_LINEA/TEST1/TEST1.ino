#define NUMSENS 8
#define TIMEPERINT 0xF9E4 //63,972 -> 63,972.5 -> cada 0.1s -> 10Hz

#define BOTON 9
#define ARR 2

#define AIN2 4
#define AIN1 5
#define BIN1 6
#define BIN2 7
#define PWMA 3
#define PWMB 11

#define KP 2
#define KI 0.01
#define KD .01
#define SETPOINT 0
#define OUTMAX 120
#define OUTMIN -120
float ITerm = 0;
int ultEntrada = 0;

const byte dirSens[NUMSENS] = {A0, A1, A2, A3, A4, A5, A6, A7};
int umbral[NUMSENS] = {200, 200, 200, 200, 200, 200, 200, 200};
byte sens = 0;

void setup(){

    pinMode(BOTON, INPUT); pinMode(ARR, INPUT);
    pinMode(AIN2, OUTPUT); pinMode(AIN1, OUTPUT); pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);

}

void loop(){

    tracking2();

}

void tracking2(void){

    while(1){

        for(byte i = 0; i < NUMSENS; ++i){
            int valSens = analogRead(dirSens[i]);
            (valSens >= umbral[i]) ? (sens |= (1 << i)) : (sens &= ~(1 << i));
        }

        if(sens & 0B10000001) adelante(OUTMAX, OUTMAX, 100);
        else if((sens & 0B10000000) && !(sens & 0B01111111)) derecha(OUTMAX, OUTMAX, 200);
        else if((sens & 0B00000001) && !(sens & 0B11111110)) izquierda(OUTMAX, OUTMAX, 200);

        int8_t posicion = ponderacion();
        int error = SETPOINT - posicion;
        ITerm += (KI * error);
        if(ITerm > OUTMAX) ITerm = OUTMAX;
        else if(ITerm < OUTMIN) ITerm = OUTMIN;
        int dEntrada = (posicion - ultEntrada);

        int salida = (error * KP) + ((int) ITerm) - ((int) (dEntrada * KD));
        if(salida > OUTMAX) salida = OUTMAX;
        else if(salida < OUTMIN) salida = OUTMIN;

        salMot(salida);

        ultEntrada = posicion;
    }

    ITerm = ultEntrada = 0;

}

int8_t ponderacion(void){

    static const int8_t pond[NUMSENS] = {-30, -20, -10, 0, 0, 10, 20, 30};
    if(!sens) return 0;
    int8_t sumPond = 0, sumAS = 0;
    for(byte i = 0; i < NUMSENS; ++i){
        if(bitRead(sens, i)){
            sumPond += pond[i];
            ++sumAS;
        }
    }

    sumPond /= sumAS;

    return sumPond;
}

void salMot(int valMot){

    int motLeft = 0, motRgt = 0;

    if(valMot < 0){
        motLeft = (OUTMIN + (valMot * (-2)));
        motRgt = -OUTMIN;

        if((valMot * 2) < OUTMIN){
            izquierda(motLeft, motRgt, 2);
        } else{
            motLeft *= -1;
            adelante(motLeft, motRgt, 2);
        }
    } else if(!valMot){
        adelante(OUTMAX, OUTMAX, 2);
    } else if(valMot > 0){
        motLeft = OUTMAX;
        motRgt = (OUTMAX - (valMot * (-2)));
 
        if((valMot * 2) > OUTMAX){
            derecha(motLeft, motRgt, 2);
        } else{
            motRgt *= -1;
            adelante(motLeft, motRgt, 2);
        }
    }

}

void adelante(byte PWML, byte PWMR, int T){
    digitalWrite(AIN2, LOW);
    digitalWrite(AIN1, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMA, PWML);
    analogWrite(PWMB, PWMR);
    if(!T) return;
    unsigned long taim = millis();
    do{
    }while((millis() - taim) <= T);

}

void izquierda(byte PWML, byte PWMR, int T){
    digitalWrite(AIN2, HIGH);
    digitalWrite(AIN1, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMA, PWML);
    analogWrite(PWMB, PWMR);
    if(!T) return;
    unsigned long taim = millis();
    do{
    }while((millis() - taim) <= T);
}

void derecha(byte PWML, byte PWMR, int T){
    digitalWrite(AIN2, LOW);
    digitalWrite(AIN1, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMA, PWML);
    analogWrite(PWMB, PWMR);
    if(!T) return;
    unsigned long taim = millis();
    do{
    }while((millis() - taim) <= T);
}

void paro(void){
    digitalWrite(AIN2, LOW);
    digitalWrite(AIN1, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN1, LOW);
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}
