#include <SoftwareSerial.h>
#include <Servo.h>
#include <FastLED.h>

//#define realFastRead(pin, numPin) (((pin) >> (numPin)) & 0x01)
//#define fastRead(pin_numPin) realFastRead(pin_numPin)
#define realFastWrite(port, numPin, value) ((value) ? ((port) |= (1U << (numPin))) : ((port) &= ~(1U << (numPin))))
#define fastWrite(port_numPin, value) realFastWrite(port_numPin, value)
#define NUM_LEDS 1

// CONEXIONES PARA EL BLUETOOTH.
#define bluetoothTx 10
#define bluetoothRx 9

#define DATA_PIN 11
#define CLOCK_PIN 13

// MOTOR 1.
#define Motor1A PORTD, 4
#define Motor1B PORTD, 3
#define PWMA 5
#define STBY PORTD, 2

// MOTOR 2.
#define Motor2A PORTD, 7
#define Motor2B PORTB, 0
#define PWMB 6

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
CRGB leds[NUM_LEDS];

void setup(){
    FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
    bluetooth.begin(115200);
    bluetooth.print("$$$");
    bluetooth.println("U,9600,N");
    bluetooth.begin(9600);

    DDRD |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 7);
    DDRB |= (1 << 0);
    fastWrite(STBY, HIGH);
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}

signed char flag1 = -1;

void loop(){
    if(bluetooth.available()){
    char toSend = (char)bluetooth.read();

        if(toSend == 'S'){
            flag1 = 0;

            fastWrite(Motor1A, LOW);
            fastWrite(Motor1B, LOW);
            analogWrite(PWMA, 0);
            fastWrite(Motor2A, LOW);
            fastWrite(Motor2B, LOW);
            analogWrite(PWMB, 0);
        } else if(toSend == 'B' || toSend == 'H' || toSend == 'J'){ //if( toSend == 'F' || toSend == 'G' || toSend == 'I')
            if(flag1 != 1){
                // ESTOS HARAN QUE VAYA PARA ADELANTE EL CARRITO.
                flag1 = 1;
                fastWrite(Motor1A, HIGH);
                fastWrite(Motor1B, LOW);
                analogWrite(PWMA, 255);
                fastWrite(Motor2A, HIGH);
                fastWrite(Motor2B, LOW);
                analogWrite(PWMB, 255);
                leds[0] = CRGB(255,0,0);
                FastLED.show();
            }
        } else if(toSend == 'F' || toSend == 'G' || toSend == 'I'){
            if(flag1 != 2){
            // ESTOS HARAN LA REVERSA DEL CARRITO.
            flag1 = 2;
            fastWrite(Motor1B, HIGH);
            fastWrite(Motor1A, LOW);
            analogWrite(PWMA, 255);
            fastWrite(Motor2B, HIGH);
            fastWrite(Motor2A, LOW);
            analogWrite(PWMB, 255);
            leds[0] = CRGB(0,255,0);
            FastLED.show();
            }
        } else if(toSend == 'L' || toSend == 'G' || toSend == 'H'){
            if(flag1 != 3){
                // ESTOS HARAN QUE GIRE HACIA LA IZQUIERDA.
                flag1 = 3;
                fastWrite(Motor2B, HIGH);
                fastWrite(Motor2A, LOW);
                analogWrite(PWMA, 255);
                fastWrite(Motor1A, HIGH);
                fastWrite(Motor1B, LOW);
                analogWrite(PWMB, 255);
                leds[0] = CRGB(0,0,255);
                FastLED.show();
            }
        } else if(toSend == 'R' || toSend == 'I' || toSend == 'J'){
            if(flag1 != 4){
                // ESTOS HARAN QUE GIRE HACIA LA DERECHA.
                flag1 = 4;
                fastWrite(Motor1B, HIGH);
                fastWrite(Motor1A, LOW);
                analogWrite(PWMA, 255);
                fastWrite(Motor2A, HIGH);
                fastWrite(Motor2B, LOW);
                analogWrite(PWMB, 255);
                leds[0] = CRGB(255,255,255);
                FastLED.show();
            }
        } else{
            if(flag1 != 5){
                flag1 = 5;
                fastWrite(Motor1A, HIGH);
                fastWrite(Motor1B, HIGH);
                analogWrite(PWMA, 0);  
                fastWrite (Motor2B, HIGH);
                fastWrite(Motor2A, HIGH);
                analogWrite(PWMB, 0); 
                leds[0] = CRGB(255,255,0);
                FastLED.show();
            }
        }
    }
}
