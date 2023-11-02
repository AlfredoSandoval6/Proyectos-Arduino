#define LED1 0
#define LED2 1

#define fastRead(pin, numPin) (((pin) >> (numPin)) & 0x01)
#define fastWrite(port, numPin, val) ((val) ? bitSet(port, numPin) : bitClear(port, numPin))

void setup()
{
    DDRD = 0B00000000;

    Serial.begin(9600);
}

void loop()
{
    if(fastRead(PIND, 7)) Serial.println(F("Hi!"));
    delay(345);
}