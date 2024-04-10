#include "LiquidCrystal_I2C.h"
#include "fastPins.h"

//BOTONES
#define BTON1 1
#define BTON2 2
#define BTON3 3
#define BTON4 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){
    lcd.init();
    lcd.backlight();
    lcd.clear();
/*    TCCR1A = 0;
    TCCR1B |= (1 << CS10) | (1 << CS12);
    TCNT1 = 0;
    TIMSK1 |= (1 << TOIE1);
*/
}

void loop(){
    lcd.setCursor(0, 0);
    lcd.print("Pasa");
    lcd.setCursor(0, 1);
    lcd.print("lamas 4");

}

void lect_ste_button0(){

}

ISR(TIMER1_OVF_vect){ //INTERRUPCIÓN DEL TIMER 1.
    TCNT1 = 0;

}