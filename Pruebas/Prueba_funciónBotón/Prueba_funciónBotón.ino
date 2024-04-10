#define TIME_INT 0xE17C

void setup(){
    pinMode(7, INPUT_PULLUP);
    pinMode(13, OUTPUT);

    TCCR1A = 0;
    TCCR1B |= (1 << CS10) | (1 << CS12);
    TCNT1 = 0;
    TIMSK1 |= (1 << TOIE1);

    Serial.begin(9600);
}

void loop(){
    static uint8_t ste_Button = 0;
    if(button_press_event0()){
        ste_Button = !ste_Button;
        delay(10);
        if(ste_Button) Serial.println("Activado!");
        else if(!ste_Button) Serial.println("Desctivado!");
    }

    if(ste_Button){
        TCCR1B |= (1 << CS10) | (1 << CS12);
        TCNT1 = TIME_INT;
    } else{
        TCCR1B = 0;
        TCNT1 = 0;
        digitalWrite(13, LOW);
    
    }
}

uint8_t button_press_event0(){
    static uint8_t estado_boton = 1;
    uint8_t out, lectura;
    out = 0;
    lectura = digitalRead(7);

    if((!lectura) && estado_boton){
        out = 1;
    }

    estado_boton = lectura;

    return out;
}

ISR(TIMER1_OVF_vect){ //INTERRUPCIÓN DEL TIMER 1.
    TCNT1 = TIME_INT;
    static uint8_t ste_Led = LOW;
    digitalWrite(13, ste_Led);
    ste_Led = !ste_Led;
}