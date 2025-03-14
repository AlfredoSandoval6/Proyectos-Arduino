#define NUMSENS 8
#define TIMEPERINT 0xF9E4 //63,972 -> 63,972.5 -> cada 0.1s -> 10Hz

#define BOTON 9
#define ARR 2
#define ARR_READY 8

#define AIN2 4
#define AIN1 5
#define BIN1 6
#define BIN2 7
#define PWMA 3
#define PWMB 11

#define KP 2
#define KI 0.008
#define KD 0.1
#define SETPOINT 0
#define OUTMAX 150
#define OUTMIN -150
float ITerm = 0;
int ultEntrada = 0;

const byte dirSens[NUMSENS] = {A0, A1, A2, A3, A4, A5, A6, A7};
int umbral[NUMSENS];
bool FLGMNOMX = false;
bool FLGCTD = false;
byte sens = 0;
volatile byte steARR = false;

void cSteARR(void);
void cal(void);
void tracking1(void);
void tracking2(void);
int8_t ponderacion(void);
void salMot(int valMot);
void adelante(byte PWML, byte PWMR, int T);
void izquierda(byte PWML, byte PWMR, int T);
void derecha(byte PWML, byte PWMR, int T);
void paro(void);

void setup(){

    pinMode(BOTON, INPUT); pinMode(ARR, INPUT);
    pinMode(AIN2, OUTPUT); pinMode(AIN1, OUTPUT); pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(ARR), cSteARR, CHANGE);

    Serial.begin(115200);

}

void loop(){

    if(!FLGCTD){
        paro();
        cal();
    }

    if(steARR){
        if(FLGMNOMX) tracking1(); else tracking2();
        FLGCTD = false;
    }

}

void cSteARR(void){
    steARR = !steARR;
}

void cal(void){

    unsigned long taim = millis();
    int lect[NUMSENS], Max[NUMSENS], Min[NUMSENS];
    byte findLine = 0;

    do{}while(!digitalRead(ARR_READY));
    Serial.println(F("MÁXIMOS Y MINIMOS:"));
    digitalWrite(LED_BUILTIN, HIGH);
    for(byte i = 0; i < NUMSENS; ++i) Max[i] = Min[i] = analogRead(dirSens[i]);
    do{

        int valSens = 0;

        for(byte i = 0; i < NUMSENS; ++i){
            valSens = analogRead(dirSens[i]);
            if(valSens > Max[i]) Max[i] = valSens;
            else if(valSens < Min[i]) Min[i] = valSens;
        }

    }while((millis() - taim) <= 5000);
    digitalWrite(LED_BUILTIN, LOW);

    for(byte i = 0; i < NUMSENS; ++i){
        Serial.print(Max[i]); Serial.print("\t"); Serial.println(Min[i]);
    }

    do{}while(!digitalRead(ARR_READY));
    Serial.println(F("UMBRAL"));
    digitalWrite(LED_BUILTIN, HIGH);
    taim = millis();
    do{
        for(byte i = 0; i < NUMSENS; ++i){
            lect[i] = analogRead(dirSens[i]);
        }
    }while((millis() - taim) <= 1000);
    digitalWrite(LED_BUILTIN, LOW);

    for(byte i = 0; i < NUMSENS; ++i){
        umbral[i] = ((Max[i] + Min[i]) >> 1); //    (a >> 1) == (a / 2)
    }

    for(byte i = 0; i < NUMSENS; ++i){
        if(lect[i] > umbral[i]){
            ++findLine;
        }
    }

    if(findLine > 4) FLGMNOMX = true;
    else if(findLine < 4) FLGMNOMX = false;

    FLGCTD = true;

    for(byte i = 0; i < NUMSENS; ++i){
        Serial.print(umbral[i]); Serial.print("\t"); Serial.println(lect[i]);
    }

    if(findLine > 4) Serial.println(F("En línea blanca"));
    else if(findLine < 4) Serial.println(F("En línea negra"));

}

void tracking1(void){

    while(steARR){
        //Serial.println("HEy");
        for(byte i = 0; i < NUMSENS; ++i){
            int valSens = analogRead(dirSens[i]);
            (valSens <= umbral[i]) ? (sens |= (1 << i)) : (sens &= ~(1 << i));
        }

        if((sens & 0B00000001) || (sens & 0B10000000)){
            if(sens & 0B10000001){
                adelante(OUTMAX, OUTMAX, 200);
            } else if(sens & 0B00000001){
                izquierda(OUTMAX, OUTMAX, 200);
            } else if(sens & 0B10000000){
                derecha(OUTMAX, OUTMAX, 200);
            }

            ITerm = ultEntrada = 0;
        }

        int8_t posicion = ponderacion();
        if(!sens) continue;
        int error = SETPOINT - posicion;
        ITerm += (KI * error);
        if(ITerm > OUTMAX) ITerm = OUTMAX;
        else if(ITerm < OUTMIN) ITerm = OUTMIN;
        int dEntrada = (posicion - ultEntrada);

        int salida = (error * KP) + ((int) (ITerm + (dEntrada * KD)));
        if(salida > OUTMAX) salida = OUTMAX;
        else if(salida < OUTMIN) salida = OUTMIN;
        Serial.println(salida);

        salMot(salida);

        ultEntrada = posicion;
    }

    ITerm = ultEntrada = 0;

}

void tracking2(void){

    while(steARR){

        for(byte i = 0; i < NUMSENS; ++i){
            int valSens = analogRead(dirSens[i]);
            (valSens >= umbral[i]) ? (sens |= (1 << i)) : (sens &= ~(1 << i));
        }

        int8_t posicion = ponderacion();
        if((posicion == 30)){izquierda(OUTMAX, OUTMAX, 40); continue;}
        else if((posicion == -30)){derecha(OUTMAX, OUTMAX, 40); continue;}
        int error = SETPOINT - posicion;
        ITerm += (KI * error);
        if(ITerm > OUTMAX) ITerm = OUTMAX;
        else if(ITerm < OUTMIN) ITerm = OUTMIN;
        int dEntrada = (posicion - ultEntrada);

        int salida = (error * KP) + ((int) ITerm) + ((int) (dEntrada * KD));
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
        if(!steARR) return;
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
        if(!steARR) return;
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
        if(!steARR) return;
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
