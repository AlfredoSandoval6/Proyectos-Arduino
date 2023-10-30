#define p_l_1 0
#define p_l_2 1

int currMillis, prevMillis, timeLapse = 1000;

void setup()
{
    pinMode(p_l_1, OUTPUT);
    pinMode(p_l_2, OUTPUT);

    Serial.begin(9600);
}

void loop()
{
    currMillis = millis();
    if((currMillis - prevMillis) >= timeLapse){
        prevMillis = currMillis;
        Serial.println("Tested time!");
    }
}

//Prueba exitosa: el uso de millis permite hacer toda clase de tareas mientras comprobamos que el periodo de
//"espera" haya finalizado, todo en base al uso de los milisegundos transcurridos entre un momento específico
//hasta el "esperado". Hay que tener cuidado en que las variables para almacenar los "momentos" no se desborden.