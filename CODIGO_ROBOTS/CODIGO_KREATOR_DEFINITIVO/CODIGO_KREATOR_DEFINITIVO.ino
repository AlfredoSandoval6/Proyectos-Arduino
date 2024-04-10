#include <SoftwareSerial.h>
#include <Servo.h>
#include "FastLED.h"
// CONEXIONES PARA EL BLUETOOTH.
#define NUM_LEDS 1
int bluetoothTx = 10;
int bluetoothRx = 9;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

#define DATA_PIN 11
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];
// MOTOR 1.

int Motor1A = 4;
int Motor1B = 3;
const int PWMA = 5;
int STBY = 2;
// MOTOR 2.

int Motor2A = 7;
int Motor2B = 8;
const int PWMB = 6;

void setup ()
{
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS); 
bluetooth.begin(115200);
bluetooth.print("$$$");
delay(1);
bluetooth.println("U,9600,N");
bluetooth.begin(9600);

pinMode( Motor1A, OUTPUT );
pinMode( Motor2A, OUTPUT );
pinMode( Motor1B, OUTPUT );
pinMode( Motor2B, OUTPUT );
pinMode( STBY , OUTPUT );


digitalWrite( Motor1A, LOW );
digitalWrite( Motor2A, LOW );
analogWrite(PWMA, 0);
digitalWrite( Motor1B, LOW );
digitalWrite( Motor2B, LOW );
analogWrite(PWMB, 0);
digitalWrite( STBY, HIGH );
}

int flag1 = -1;
int flag2 = -1;

void loop()
{
if(bluetooth.available())
{
char toSend = (char)bluetooth.read();
if(toSend == 'S')
{
  
flag1 = 0;
flag2 = 0;

digitalWrite( Motor1A, LOW);
analogWrite( Motor1B, LOW);
analogWrite(PWMA, 0);

digitalWrite( Motor2A, LOW),
analogWrite( Motor2B, LOW);
analogWrite(PWMB, 0);

}
if( toSend == 'B' || toSend == 'H' || toSend == 'J')//if( toSend == 'F' || toSend == 'G' || toSend == 'I')
{
if (flag1 != 1)
{
// ESTOS HARAN QUE VAYA PARA ADELANTE EL CARRITO.
flag1 = 1;
digitalWrite( Motor1A, HIGH);
analogWrite( Motor1B, LOW );
analogWrite(PWMA, 255);
digitalWrite( Motor2A, HIGH);
analogWrite( Motor2B, LOW );
analogWrite(PWMB, 255);
leds[0] = CRGB(255,0,0);
FastLED.show();
}
}
if(toSend == 'F' || toSend == 'G' || toSend == 'I')
{
if(flag1 != 2)
{
// ESTOS HARAN LA REVERSA DEL CARRITO.
flag1 = 2;
digitalWrite( Motor1B, HIGH);
analogWrite( Motor1A, LOW );
analogWrite(PWMA, 255);
digitalWrite( Motor2B, HIGH);
analogWrite( Motor2A, LOW );
analogWrite(PWMB, 255);
leds[0] = CRGB(0,255,0);
FastLED.show();
}
}
if(toSend == 'L' || toSend == 'G' || toSend == 'H')
{
if(flag2 != 1)
{
// ESTOS HARAN QUE GIRE HACIA LA IZQUIERDA.
flag2 = 1;
digitalWrite( Motor2B, HIGH);
analogWrite( Motor2A, LOW );
analogWrite(PWMA, 255);
digitalWrite( Motor1A, HIGH);
analogWrite( Motor1B, LOW );
analogWrite(PWMB, 255);
leds[0] = CRGB(0,0,255);
FastLED.show();
}
}
else
if(toSend == 'R' || toSend == 'I' || toSend == 'J')
{
if(flag2 != 2)
{
// ESTOS HARAN QUE GIRE HACIA LA DERECHA.
flag2 = 2;
digitalWrite( Motor1B, HIGH);
analogWrite( Motor1A, LOW );
analogWrite(PWMA, 255);
digitalWrite( Motor2A, HIGH);
analogWrite( Motor2B, LOW );
analogWrite(PWMB, 255);
leds[0] = CRGB(255,255,255);
FastLED.show();
}
}
else
{
if(flag2 != 3) 
{
flag2 = 3;
digitalWrite ( Motor1A, HIGH);
analogWrite ( Motor1B, HIGH);
analogWrite(PWMA, 0);  
digitalWrite ( Motor2B, HIGH);
analogWrite ( Motor2A, HIGH);
analogWrite(PWMB, 0); 
leds[0] = CRGB(255,255,0);
 FastLED.show();
}
}
}
}
