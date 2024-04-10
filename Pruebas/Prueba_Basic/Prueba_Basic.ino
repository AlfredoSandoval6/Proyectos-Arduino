//PROGRAMA PARA LA LECTURA DEL SENSOR

#define PIN_SENSOR 0

int lect_sen = 0;

void setup(){
    Serial.begin(9600);
    pinMode(PIN_SENSOR, INPUT);
}

void loop(){
    lect_sen = digitalRead(PIN_SENSOR);
    if(lect_sen == 1){
        Serial.println(F("SENSOR DETECTANDO."));
    } else if(lect_sen == 0){
        Serial.println(F("SENSOR NO DETECTANDO."));
    }
    /*SI EL VALOR QUE ARROJA EL SENSOR ES 1
    ENTONCES SE ENVÍA LA SEÑAL QUE ESTÁ DETECTANDO POR EL PUERTO SERIE
    SI EL VALOR QUE ARROJA EL SENSOR ES 0
    ENTONCES SE ENVÍA LA SEÑAL QUE NO ESTÁ DETECTANDO POR EL PUERTO SERIE*/
}