#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "HCSR04.h"

#define BOMBA 7

#define TRIGGERPIN 6
#define ECHOPIN 5
#define BOTON1 3
#define BOTON2 4

bool button1 = false;
bool button2 = false;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

float distancia = 0, valor = 0;

void setup(){

    lcd.begin(16, 2);
    lcd.home();
    lcd.write("NIVEL DE AGUA:");
    lcd.setCursor(7, 1);
    lcd.print("%");

    HCSR04.begin(TRIGGERPIN, ECHOPIN);
    Serial.begin(9600);

    pinMode(BOMBA, OUTPUT);
    pinMode(BOTON1, INPUT);
    pinMode(BOTON2, INPUT);

}

void loop(){

    distancia = (HCSR04.measureDistanceCm())[0];
    //Serial.println(distancia);
    valor = (distancia - 21.3) * (100.0 - 0) / (7.4 - 21.3) + 0;
    //Serial.println(valor);
    lcd.setCursor(0, 1);
    lcd.print(valor);
    delay(250);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);

    if(digitalRead(BOTON1)) button1 = true;
    if(digitalRead(BOTON2)) button2 = true;

    if(button1 && button2){
        button1 = button2 = false;
    }
    delay(100);

    if(button1 && (!button2) && (valor > 46.0)){
        digitalWrite(BOMBA, LOW);
    } else if(button1 && (!button2) && (valor < 46.0)){
        digitalWrite(BOMBA, HIGH);
    }

    if((!button1) && button2 && (valor > 96.0)){
        digitalWrite(BOMBA, LOW);
    } else if((!button1) && button2 && (valor < 96.0)){
        digitalWrite(BOMBA, HIGH);
    }

    if(button1) Serial.println("LLENADO AL 50");
    else if(button2) Serial.println("LLENADO AL 100");
    else Serial.println("NADA");

}