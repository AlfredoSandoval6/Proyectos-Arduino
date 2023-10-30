#define P_INT 2
#define P_HIGH 8

int currMillis = 0, beginMillis = 0, timeLapse = 3000;

void setup()
{
    pinMode(P_INT, INPUT_PULLUP);
    pinMode(P_HIGH, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(P_INT), message, CHANGE);

    Serial.begin(9600);
}

void loop()
{
    digitalWrite(P_HIGH, HIGH);
    Serial.println(F("Hey!"));
    do{
        currMillis = millis();
    }while((currMillis - beginMillis) <= timeLapse);
    beginMillis = currMillis;
    digitalWrite(P_HIGH, LOW);

    detachInterrupt(digitalPinToInterrupt(P_INT));
}

void message(){
    Serial.println(F("What's up!"));
}

//Prueba exitosa: la ejecución de interrupciones se realiza de manera espontánea. Una vez que un pin se 'desmarca'
//para interrupción, no se realizan sus operaciones asociadas ni se consideran los posibles cambios en el estado
//del pin. 'INPUT_PULLUP' es el equivalente a poner a un pin en estado de alta impedancia (declararlo como INPUT),
//y además hacer un 'digitalWrite()' para activar las resistencias internas de 20kOhms.