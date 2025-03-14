#define SENLL 0
#define SENL 0
#define SENF 0
#define SENR 0
#define SENLR 0
#define SENPL 0

#define SENPR 0
#define ARR 2
#define PIN_SGY1 0
#define PIN_SGY2 0

#define MOTLA 0
#define PWMA 0
#define MOTLB 0
#define MOTRA 0
#define PWMB 0
#define MOTRB 0

#define NUM_SENS 5
#define SETPOINT 30
#define KP 14
#define KI 0.1
#define KD 12

#define SETPOINT 20
#define MIN_OUT -250
#define MAX_OUT 250
#define NO_SPEED 0
#define MAX_SPEED 250

#define IS_LEFT 0
#define IS_FRONT 1
#define IS_RIGHT 2
#define IS_FLAN 3

volatile byte steARR = 0;
byte sens = 0;

void cSteARR();
void selSgy();
void sgy1();
void sgy2();
void sgy3();
void sgy4();
void lectSens();
void analisis();
void busqueda(byte goFind);
void control();
byte ponderacion();
void adelante(byte PWML, byte PWMR, int T);
void izquierda(byte PWML, byte PWMR, int T);
void derecha(byte PWML, byte PWMR, int T);
void atras(byte PWM, int T);
void paro(int T);

void (*SGTY[])() = {sgy1, sgy2, sgy3, sgy4};

void setup(){

    pinMode(SENL, INPUT);
    pinMode(SENF, INPUT);
    pinMode(SENR, INPUT);
    pinMode(PIN_SGY1, INPUT);
    pinMode(PIN_SGY2, INPUT);
    pinMode(ARR, INPUT);

    pinMode(MOTLA, OUTPUT);
    pinMode(MOTLB, OUTPUT);
    pinMode(MOTRA, OUTPUT);
    pinMode(MOTRB, OUTPUT);
    paro(0);

    attachInterrupt(digitalPinToInterrupt(ARR), cSteARR, CHANGE);

}

void loop(){

    if(steARR){
        selSgy();
        paro(1000);
    }

}

void cSteARR(){

    steARR = !steARR;

}

void lectSens(){

    (digitalRead(SENLL)) ? (sens |= (1 << 0)) : (sens &= ~(1 << 0));
    (digitalRead(SENL)) ? (sens |= (1 << 1)) : (sens &= ~(1 << 1));
    (digitalRead(SENF)) ? (sens |= (1 << 2)) : (sens &= ~(1 << 2));
    (digitalRead(SENR)) ? (sens |= (1 << 3)) : (sens &= ~(1 << 3));
    (digitalRead(SENLR)) ? (sens |= (1 << 4)) : (sens &= ~(1 << 4));
    (analogRead(SENPL) < 100) ? (sens |= (1 << 5)) : (sens &= ~(1 << 5));
    (analogRead(SENPR) < 100) ? (sens |= (1 << 6)) : (sens &= ~(1 << 6));

}

void selSgy(){

    byte sgy = 0;

    sgy |= ((digitalRead(PIN_SGY1)) << 0);
    sgy |= ((digitalRead(PIN_SGY2)) << 1);

    (*SGTY[sgy])();

    analisis();

}

void sgy1(){

}

void sgy2(){

}

void sgy3(){

}

void sgy4(){

}

void analisis(){

    do{

        static byte antSens = 0;
        lectSens();
        if(sens & 0B01100000){
            byte fSens = (sens & 0B001100000);
            if(fSens == 0B001100000){
                atras(180, 180);
                izquierda(180, 180, 180);
            } else if(fSens == 0B00100000){
                atras(180, 180);
                izquierda(180, 180, 90);
            } else if(fSens == 0B01000000){
                atras(180, 180);
                derecha(180, 180, 90);
            }
            lectSens();
            antSens = 0;
        }

        if((!sens) && antSens){

            byte blockA = 0, blockB = 0, blockF = 0;
            blockA += (((antSens >> 4) & 1U) * 2) + ((antSens >> 3) & 1U);
            blockB += ((antSens >> 1) & 1U) + (((antSens >> 0) & 1U) * 2);
            blockF = ((antSens >> 2) & 1U);
            if(blockA > blockB) busqueda(IS_RIGHT);
            else if(blockA < blockB) busqueda(IS_LEFT);
            else if(((blockA == blockB) && blockA) || blockF) busqueda(IS_FRONT);
        } else if(!sens) busqueda(IS_FLAN);
        antSens = (sens & 0B00011111);

        if(sens & 0B00011111) control();

    }while(steARR);

}

void busqueda(byte goFind){

    if(goFind != IS_FLAN){
        if(goFind == IS_LEFT){
            izquierda(180, 180, 0);
        } else if(goFind == IS_FRONT){
            adelante(180, 180, 0);
        } else if(goFind == IS_RIGHT){
            derecha(180, 180, 0);
        }
        unsigned int taim = millis();
        do{

            lectSens();
            if((sens) || (!steARR)) return;

        }while((millis() - taim) <= 180);
    } else{
        adelante(50, 50, 0);
        do{
            lectSens();
            if(!steARR) return;
        }while(!sens);
    }
}

void control(){

        static int8_t ultEntrada = 0;
        static float sumInt = 0;
        static int8_t error6 = 0, error5 = 0, error4 = 0, error3 = 0, error2 = 0, error1 = 0;

    do{

        if(!steARR) return;

        byte posicion = ponderacion();

        int8_t error = SETPOINT - posicion;
        int8_t dEntrada = error - ultEntrada;
        error6 = error5, error5 = error4, error4 = error3, error3 = error2, error2 = error1, error1 = error;
        sumInt = ((error6 + error5 + error4 + error3 + error2 + error1 + error) * KI);
        if(sumInt > MAX_OUT) sumInt = MAX_OUT;
        else if(sumInt < MIN_OUT) sumInt = MIN_OUT;
        int salida = (error * KP) + ((int) sumInt) + (dEntrada * KD);
        if(salida > MAX_OUT) salida = MAX_OUT;
        else if (salida < MIN_OUT) salida = MIN_OUT;
        if(salida < 0){salida *= -1; izquierda(salida, salida, 2);}
        else if(!salida) adelante(MAX_SPEED, MAX_SPEED, 2);
        else if (salida > 0) derecha(salida, salida, 2);

        ultEntrada = dEntrada;
        lectSens();

        delay(1);

    }while(sens & 0B00011111);

    error6 = error5 = error4 = error3 = error2 = error1 = ultEntrada = 0;
    sumInt = 0;

}

byte ponderacion(){

    byte amnt_det = 10, n_det = 0;

    for(byte i = 0; i < NUM_SENS; ++i){
        if(bitRead(sens, i)){
            amnt_det += 10;
            ++n_det;
        }
    }

    amnt_det /= n_det;

    return amnt_det;

}

void adelante(byte PWML, byte PWMR, int T){

    digitalWrite(MOTLA, HIGH);
    digitalWrite(MOTLB, LOW);
    analogWrite(PWMA, PWML);
    digitalWrite(MOTRA, HIGH);
    digitalWrite(MOTRB, LOW);
    analogWrite(PWMB, PWMR);
    if(!T) return;
    unsigned int taim = millis();
    do{if(!steARR) return;}while((millis() - taim) <= T);

}

void izquierda(byte PWML, byte PWMR, int T){

    digitalWrite(MOTLA, LOW);
    digitalWrite(MOTLB, HIGH);
    analogWrite(PWMA, PWML);
    digitalWrite(MOTRA, HIGH);
    digitalWrite(MOTRB, LOW);
    analogWrite(PWMB, PWMR);
    if(!T) return;
    unsigned int taim = millis();
    do{if(!steARR) return;}while((millis() - taim) <= T);

}

void derecha(byte PWML, byte PWMR, int T){

    digitalWrite(MOTLA, HIGH);
    digitalWrite(MOTLB, LOW);
    analogWrite(PWMA, PWML);
    digitalWrite(MOTRA, LOW);
    digitalWrite(MOTRB, HIGH);
    analogWrite(PWMB, PWMR);
    if(!T) return;
    unsigned int taim = millis();
    do{if(!steARR) return;}while((millis() - taim) <= T);

}

void atras(byte PWM, int T){

    digitalWrite(MOTLA, LOW);
    digitalWrite(MOTLB, HIGH);
    analogWrite(PWMA, PWM);
    digitalWrite(MOTRA, LOW);
    digitalWrite(MOTRB, HIGH);
    analogWrite(PWMB, PWM);
    if(!T) return;
    unsigned int taim = millis();
    do{if(!steARR) return;}while((millis() - taim) <= T);

}

void paro(int T){

    digitalWrite(MOTLA, LOW);
    digitalWrite(MOTLB, LOW);
    analogWrite(PWMA, 0);
    digitalWrite(MOTRA, LOW);
    digitalWrite(MOTRB, LOW);
    analogWrite(PWMB, 0);
    if(!T) return;
    unsigned int taim = millis();
    do{if(!steARR) return;}while((millis() - taim) <= T);

}