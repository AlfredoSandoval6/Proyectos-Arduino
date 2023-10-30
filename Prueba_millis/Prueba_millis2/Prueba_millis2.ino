int currMillis = 0, prevMillis = 0, timeLapse = 3000;
const byte p_leds[2] = {0, 1};
int leds = 0B00000011;

void setup()
{
    for(int i = 0; i < 2; ++i) pinMode(p_leds[i], OUTPUT);
    //Serial.begin(9600);
}

void loop()
{
    prevMillis = millis();
    digitalWrite(p_leds[0], bitRead(leds, 0)); digitalWrite(p_leds[1], bitRead(leds, 1));
    do{
        currMillis = millis();
    }while((currMillis - prevMillis) <= timeLapse);
    //Serial.println("Time 1!");
    prevMillis = currMillis;
    digitalWrite(p_leds[0], bitRead(leds, 2)); digitalWrite(p_leds[0], bitRead(leds, 3));
    do{
        currMillis = millis();
    }while((currMillis - prevMillis) <= timeLapse);
    //Serial.println("Time 2!");
}

//Prueba exitosa: se ha logrado hacer funcionar el encendido y apagado de (un) led, en base al uso de la
//función de los milisegundos transcurridos desde el arranque del programa. Al parecer, el abrir el puerto serie
//cambia el comportamiento del arduino, al parecer es porque el pin digital "0" está ligado al puerto de
//comunicación serie. La parte del "while" funciona tomando la diferencia existente entre el "punto base" y el
//"punto actual", para comprobar si sigue siendo menor al lapso de tiempo esperado, para seguir comprobando el
//tiempo transcurrido. Antes de cada "do - while", se enciende o apaga el led. La función bitRead funciona
//correctamente como se esperaba, devolviendo "1" o "0" de acuerdo al valor del bit en el byte (int) ingresada
//en la macro.