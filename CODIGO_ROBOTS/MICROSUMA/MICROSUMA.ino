#define SENL 0
#define SENF 0
#define SENR 0
#define SENPL 0
#define SENPR 0
#define ARR 0
#define PIN_SGY1 0
#define PIN_SGY2 0

#define MOTLA 0
#define PWMA 0
#define MOTLB 0
#define MOTRA 0
#define PWMB 0
#define MOTRB 0

#define NUM_SENS 3
#define SETPOINT 20
#define KP 12
#define KI 0.1
#define KD 12

#define SETPOINT 20
#define MIN_OUT -250
#define MAX_OUT 250
#define NO_SPEED 0
#define MAX_SPEED 250

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
void busqueda();
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
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    paro(0);

    attachInterrupt(digitalPinToInterrupt(ARR), cSteARR, CHANGE);

}

void loop(){

    if(steARR){
        selSgy();
        paro(0);
    }

}

void cSteARR(){

    steARR = !steARR;

}

void selSgy(){

    byte sgy = 0;

    sgy |= ((digitalRead(PIN_SGY1)) << 0);
    sgy |= ((digitalRead(PIN_SGY2)) << 1);

    (*SGTY[sgy])();

    analisis();

}

void sgy1(){

    unsigned int taim = millis();

    for(byte i = 0; i < 7; ++i){

        if((i % 2) == 0){

            adelante(MAX_SPEED, MAX_SPEED, 0);

            do{

                lectSens();
                if(sens || (!steARR)) return;

            }while((millis() - taim) - 29);

        }

        if((i % 2) == 1){

            paro(0);

            do{

                lectSens();
                if(sens || (!steARR)) return;

            }while((millis() - taim) - 29);

        }

        taim = millis();

    }

}

void sgy2(){

    unsigned int taim = millis();

    do{

        byte acumTaim = millis() - taim;
        byte sum = 0;

        if((millis() - acumTaim) > 1){
            acumTaim = millis();
            ++sum;
            adelante(sum, sum, 0);
        }
        lectSens();
        if(sens || (!steARR)) return;

    }while((millis() - taim) < 200);

}

void sgy3(){

    unsigned int taim =  millis();

    izquierda(180, 180, 90);
    adelante(150, 50, 0);
    do{

        lectSens();
        if(sens || (!steARR));

    }while((millis() - taim) < 200);

}

void sgy4(){

    izquierda(MAX_SPEED, MAX_SPEED, 0);
    unsigned int taim = millis();

    do{

        lectSens();
        if(sens || (!steARR)) return;

    }while((millis() - taim) <= 180);

}

void lectSens(){

    (digitalRead(SENL)) ? (sens |= (1 << 0)) : (sens &= ~(1 << 0));
    (digitalRead(SENF)) ? (sens |= (1 << 1)) : (sens &= ~(1 << 1));
    (digitalRead(SENR)) ? (sens |= (1 << 2)) : (sens &= ~(1 << 2));
    (analogRead(SENPL) < 100) ? (sens |= (1 << 3)) : (sens &= ~(1 << 3));
    (analogRead(SENPR) < 100) ? (sens |= (1 << 4)) : (sens &= ~(1 << 4));

}

void analisis(){

    while(steARR){

        lectSens();

        if(sens & 0B00011000){
            if((sens & 0B00011000) == 0B00001000){
                izquierda(0, 0, 0); atras(0, 0);
            } else if((sens & 0B00011000) == 0B00010000){
                derecha(0, 0, 0); atras(0, 0);
            } else{
                atras(0, 0);
            }

            continue;

        } else if(sens & 0B00000111) control();
        else busqueda();

    }

}

void busqueda(){

    adelante(100, 100, 0);

    do{

        lectSens();
        if(sens) return;

    }while(!steARR);

}

void control(){

    do{

        static int8_t ultEntrada = 0;
        static float sumInt = 0;

        if(!steARR) return;

        byte posicion = ponderacion();

        int8_t error = SETPOINT - posicion;
        int8_t dEntrada = error - ultEntrada;
        if(sumInt > MAX_OUT) sumInt = MAX_OUT;
        else if(sumInt < MIN_OUT) sumInt = MIN_OUT;
        sumInt += (error * KI);
        int salida = (error * KP) + ( (int) sumInt) + (dEntrada * KD);
        if(salida > MAX_OUT) salida = MAX_OUT;
        else if (salida < MIN_OUT) salida = MIN_OUT;
        if(salida < 0){salida *= -1; izquierda(salida, salida, 2);}
        else if(!salida) adelante(MAX_SPEED, MAX_SPEED, 2);
        else if (salida > 0) derecha(salida, salida, 2);

        ultEntrada = dEntrada;
        lectSens();

    }while(sens & 0B00000111);

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