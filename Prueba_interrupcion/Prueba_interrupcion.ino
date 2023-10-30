#define PIN_INT 2
#define PIN_LED 0
#define TRIGER 8

int currMillis = 0, prevMillis = 0, timeLapse = 3000;
volatile byte k;

void setup()
{
    pinMode(PIN_INT, INPUT);
    pinMode(TRIGER, OUTPUT);
    pinMode(PIN_LED, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_INT), message, CHANGE);
}

void loop()
{
    prevMillis = millis();
    digitalWrite(TRIGER, HIGH);
    do{
        currMillis = millis();
    }while((currMillis- prevMillis) <= timeLapse);
    prevMillis = currMillis;
    digitalWrite(TRIGER, LOW);
    do{
        currMillis = millis();
    }while((currMillis- prevMillis) <= timeLapse);
}

void message(){
    digitalWrite(PIN_LED, k);
    k = !k;
}

//Prueba exitosa: el uso de interrupciones requiere del establecimiento de sus tres parámetros: el vector de
//interrupción del pin establecido como interruptor, la función sin retorno ni parámetros que se ejecutará cada
//que haya una interrupción, y el tipo de evento ocurrido en el pin que se considerará gestionar para la
//interrupción. La ejecución de la interrupción es instantánea y fuera de todas las secuencias previas de código y
//procesos en el arduino. Naturalmente, la función en el parámetro de la interrupción es corta y no cuenta con
//esperas (delay()) o métodos similares (uso de millis()). Las variables que pueden llegar a ser usadas en
//interrupciones son declaradas 'volatile', de esa forma no se guardan copias en registros para operaciones y los
//valores de ésta son tomados y cambiados desde su localización en la memoria ram; además, al no ser alojadas en
//registros, no se optimiza, ya que los registros son componentes electrónicos de memoria que permiten un acceso y
//escritura de valores más veloces que trabajando en la ram. Sin guardar copias de variables de la ram a registros
//y viceversa, los procesos de cálculo y realización de proposiciones se vuelven más lentos.