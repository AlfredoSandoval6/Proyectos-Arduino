#include "FastLED.h"
#include "Servo.h"

#define MOTL 4
#define PWML 6
#define MOTR 3
#define PWMR 5

#define SENLL 16
#define SENL 19
#define SENF 18
#define SENR 17
#define SENLR 15
#define SENPL A7
#define SENPR A6
#define NUM_S 5
#define ARR 2
#define PSGY1 8
#define PSGY2 7

#define KP 14
#define KD 12
#define KI 0.0066667 //0.005
#define SETPOINT 30
#define MAX_SPEED 250
#define NO_SPEED 0


enum FLAG_SENS{
	IS_LEFT = 0,
	IS_FRONT,
	IS_RIGHT,
	IS_FLAN
};

#define NUM_LEDS 2
#define DATA_PIN 9
#define CLOCK_PIN 13
#define LED_TYPE NEOPIXEL
CRGB leds[NUM_LEDS];
/// @brief 
static uint8_t hue = 0;

Servo flag;

volatile byte vSteARR = 0;
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
    pinMode(ARR, INPUT); pinMode(PSGY1, INPUT); pinMode(PSGY2, INPUT);
    digitalWrite(MOTL, LOW); digitalWrite(MOTR, LOW);
    analogWrite(PWML, 0); analogWrite(PWMR, 0);
    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(ARR), cSteARR, CHANGE);

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    flag.attach(10);

}

void fadeall(){for(int i = 0; i < NUM_LEDS; i++){leds[i].nscale8(250);}}

void loop(){

    if(vSteARR){
        flag.write(90);
        selSgy();
        paro(0);
        flag.write(0);

    }

}

void cSteARR(){
    vSteARR = !vSteARR;
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

    if(digitalRead(PSGY1)) (sgy |= (1 << 0));
    if(digitalRead(PSGY2)) (sgy |= (1 << 1));

    (*SGTY[sgy])();

    posAnalisis();

}

void sgy1(){

}

void sgy2(){

    unsigned long taim = millis();

    adelante(87, 50, 0);
    do{
        lectura();
        if(sens || (!vSteARR)) return;
    }while((millis() - taim) <= 500);

}

void sgy3(){

    unsigned long taim = millis();

    adelante(50, 87, 0);
    do{
        lectura();
        if(sens || (!vSteARR)) return;
    }while((millis() - taim) <= 500);

}

void sgy4(){

    unsigned long taim = millis();

    adelante(25, 25, 0);
    do{
        lectura();
        if(sens || (!vSteARR)) return;
    }while((millis() - taim) <= 500);
    
}

void posAnalisis(){


	static uint8_t antSens = 0;

	while(vSteARR){

        lectura();

		if(sens & 0B01100000){
			if(sens == 0B01100000){
				atras(180, 180, 90);
			} else if(sens == 0B00100000){
				atras(180, 180, 90);
				izquierda(180, 180, 90);
			} else if(sens == 0B01000000){
				atras(180, 180, 90);
				derecha(180, 180, 90);
			}
			antSens = 0;
			lectura();
		}

		if((!sens) && antSens){

			uint8_t blockA = 0, blockB = 0, blockF;

			blockA += (((antSens >> 4) & 1U) * 2) + ((antSens >> 3) & 1U);
			blockB += ((antSens >> 1) & 1U) + (((antSens >> 0) & 1U) * 2);
			blockF += ((antSens >> 2) & 1U);

			if(blockA > blockB) bus_leo(IS_RIGHT);
			else if(blockA < blockB) bus_leo(IS_LEFT);
			else if(((blockA == blockB) && blockA) || blockF) bus_leo(IS_FRONT);

		} else if(!sens) bus_leo(IS_FLAN);

		antSens = (sens & 0B00011111);

		if(sens & 0B00011111) control();

	}

}

void bus_leo(byte goFind){
	if(goFind != IS_FLAN){
		if(goFind == IS_LEFT) izquierda(180, 180, 0);
		else if(goFind == IS_FRONT) adelante(180, 180, 0);
		else if(goFind == IS_RIGHT) derecha(180, 180, 0);

		uint32_t taim = millis();
		do{
			lectura();
			if((sens) || (!vSteARR)) return;
		}while((millis() - taim) <= 180);

	} else{
		adelante(50, 50, 0);
		do{
			lectura();
			if(sens) return;
		}while(vSteARR);
	}
}

void control(){

    static int ultEntrada = 0;
    static float ITerm = 0;

    do{

        int posicion = ponderacion();
        if((!sens) || (!vSteARR)) return;
        int error = SETPOINT - posicion;
        ITerm += (error * KI);

        if(!error) ITerm = 0;

        if(ITerm > MAX_SPEED) ITerm = MAX_SPEED;
        else if(ITerm < -MAX_SPEED) ITerm = -MAX_SPEED;
        int dEntrada = posicion - ultEntrada;

        int salida = (error * KP) + ((int) (ITerm)) - (dEntrada * KD);

        if(salida > MAX_SPEED) salida = MAX_SPEED;
        else if(salida < -MAX_SPEED) salida = -MAX_SPEED;
        if(salida < 0){
            salida *= -1;
            izquierda(salida, salida, 2);
        } else if(!salida){
            adelante(MAX_SPEED, MAX_SPEED, 2);
        } else if(salida > 0){
            derecha(salida, salida, 2);
        }

        ultEntrada = posicion;

    }while(sens & 0B00011111);

    ITerm = 0;
    ultEntrada = 0;

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
    leds[0] = CRGB::Blue;
    FastLED.show();
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
    leds[0] = CRGB::Green;
    FastLED.show();
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
    leds[0] = CRGB::Red;
    FastLED.show();
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
    leds[0] = CRGB::Purple;
    FastLED.show();
    if(!T) return;
    delay(T);

}