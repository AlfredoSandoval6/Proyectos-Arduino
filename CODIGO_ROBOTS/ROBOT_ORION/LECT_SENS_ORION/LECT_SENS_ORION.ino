#define SFLL A6
#define SFL A4
#define SFR A3
#define SFLR A0
#define SP_L A5
#define SP_R A1
#define L_SFLL analogRead(SFLL)
#define L_SFL analogRead(SFL)
#define L_SFR analogRead(SFR)
#define L_SFLR analogRead(SFLR)
#define L_SP_L analogRead(SP_L)
#define L_SP_R analogRead(SP_R)


void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.print(analogRead(A0)); Serial.print("\t");
    Serial.print(analogRead(A1)); Serial.print("\t");
    Serial.print(analogRead(A2)); Serial.print("\t");
    Serial.print(analogRead(A3)); Serial.print("\t");
    Serial.print(analogRead(A4)); Serial.print("\t");
    Serial.print(analogRead(A5)); Serial.print("\t");
    Serial.print(analogRead(A6));  Serial.println("\t");

    delay(100);

}