#define DIRL 4 //MOTOR IZQUIERDO
#define PWML 6 //PWM IZQUIERDO
#define DIRR 3 //MOTOR DERECHO
#define PWMR 5 //PWM DERECHO


void setup()
{
  pinMode(DIRL, OUTPUT);
  pinMode(DIRR, OUTPUT);
  paro(2000);

}

void loop()
{
/*    izquierda(240, 240, 130); //GIRO 180°
    paro(1);
    delay(2000);
    izquierda(240, 240, 55); //GIRO 90°
    paro(1);
    delay(2000);*/
    izquierda(240, 240, 27); //GIRO 45°
    paro(1);
    delay(2000);

    delay(5000);

    derecha(240, 240, 120);
    paro(1);
    delay(2000);
    derecha(240, 240, 60);
    paro(1);
    delay(2000);
    derecha(240, 240, 30);
    paro(1);
    delay(2000);

    delay(5000);
}

void izquierda(byte PWMI, byte PWMD, int T){

  digitalWrite(DIRL, LOW);
  analogWrite(PWML, PWMI);
  digitalWrite(DIRR, HIGH);
  analogWrite(PWMR, PWMD);
  delay(T);
  
}

void derecha(byte PWMI, byte PWMD, int T){

  digitalWrite(DIRL, HIGH);
  analogWrite(PWML, PWMI);
  digitalWrite(DIRR, LOW);
  analogWrite(PWMR, PWMD);
  delay(T);
  
}
void paro(int T){

  digitalWrite(DIRL, LOW);
  analogWrite(PWML, 0);
  digitalWrite(DIRR, LOW);
  analogWrite(PWMR, 0);
  delay(T);

}