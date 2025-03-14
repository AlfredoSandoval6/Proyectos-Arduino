unsigned int sens = 0;

#define SENLL 19
#define SENDL 9
#define SENL 18
#define SENF 17
#define SENR 16
#define SENDR 11
#define SENLR 15
#define SENPL A6
#define SENPR A7
#define NUM_S 7

void setup()
{
    pinMode(SENLL, INPUT); pinMode(SENL, INPUT); pinMode(SENF, INPUT); pinMode(SENLR, INPUT); pinMode(SENR, INPUT);
    Serial.begin(9600);

}

void loop()
{
    Serial.print(digitalRead(SENLL)); Serial.print("\t"); Serial.print(digitalRead(SENL)); Serial.print("\t");
    Serial.print(digitalRead(SENF)); Serial.print("\t"); Serial.print(digitalRead(SENLR)); Serial.print("\t");
    Serial.println(digitalRead(SENR));
    delay(100);
}