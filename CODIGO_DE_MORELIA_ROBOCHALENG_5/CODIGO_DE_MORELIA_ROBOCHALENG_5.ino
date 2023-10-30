


const int GO=11;
int val;

const int PWMA=5;
const int AIN2=4;
const int AIN1=7;
const int STBY=8;
const int BIN1=9;
const int BIN2=10;
const int PWMB=6;

void setup(){

  pinMode(PWMA,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(STBY,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(PWMB,OUTPUT);
   pinMode(GO,INPUT);
  digitalWrite(STBY,HIGH);


}

void loop (){
    val=digitalRead(GO);
int ADC_PD = analogRead(A0);
int ADC_PI = analogRead(A3);
int ADC_SHARPD = analogRead(A1);
int ADC_SHARPI = analogRead(A2);
int ADC_E18 = digitalRead (3);

if (val == HIGH){

if (ADC_PD >= 800 || ADC_PI >= 800) 
{
 
if (ADC_E18 == HIGH)
{
  adelante();
  analogWrite(PWMA,255);
  analogWrite(PWMB,255);
  delay(1000);}
    
if (ADC_SHARPD > 120)
{
  derechaP();
  analogWrite(PWMA,255);
  analogWrite(PWMB,255);
  delay(120);}
  
if (ADC_SHARPI > 120)
{
  izquierdaP();
  analogWrite(PWMA,255);
  analogWrite(PWMB,255);
  delay(120);}  

else
{ buscar();
analogWrite(PWMA,50);
analogWrite(PWMB,50);
delay(500);
analogWrite(PWMA,0);
analogWrite(PWMB,0);
delay(500);
}
}
else
{
  atras();
  analogWrite(PWMA,255);
  analogWrite(PWMB,255);
   }
}

else {
  digitalWrite(STBY, LOW);
  analogWrite(PWMA,0);
  analogWrite(PWMB,0);}
}



void adelante(){
  digitalWrite(AIN2,LOW);
  digitalWrite(AIN1,HIGH);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}

void buscar(){
   digitalWrite(AIN2,LOW);
  digitalWrite(AIN1,HIGH);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}
void atras(){
  digitalWrite(AIN2,HIGH);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
  delay(300);
  digitalWrite(AIN2,LOW);
  digitalWrite(AIN1,HIGH);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
  delay(100);
}

void derechaP(){
  digitalWrite(AIN2,HIGH);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}
void izquierdaP(){
  digitalWrite(AIN2,LOW);
  digitalWrite(AIN1,HIGH);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}
