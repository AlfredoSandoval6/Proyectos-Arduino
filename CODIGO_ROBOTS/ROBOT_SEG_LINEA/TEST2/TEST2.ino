#define BOTON 8

void setup()
{
    pinMode(BOTON, INPUT);
}

void loop()
{
    if(digitalRead(BOTON)){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
    }
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    
}