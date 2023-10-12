#include <Ultrasonic.h>

Ultrasonic ultrafront(2,3);
int distancia;

#define LED1 1
#define MoI 13

const int PWMA=6;
const int AIN2=7;
const int AIN1=8;
const int STBY=12;
const int BIN1=9;
const int BIN2=10;
const int PWMB=11;


void setup(){
{
  pinMode(PWMA,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(STBY,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(MoI,INPUT);
  digitalWrite(STBY,HIGH);
}
delay(500);
}

void loop(){

if( MoI == HIGH )

{

  {

  distancia = ultrafront.Ranging(CM);
  
  int SH1 = analogRead(A2);
  int SH2 = analogRead(A3);
  int SPi = analogRead(A0);
  int SPd = analogRead(A1);
  
  //SENSORES DE PISO
  if( SPi && SPd < 000){
 _BUSCAR();
 analogWrite(PWMA,200);
 analogWrite(PWMB,200);
}
else if ( SPi > 000){
  ATRASSUAVED();
analogWrite(PWMA,255);
analogWrite(PWMB,255);
}
else if ( SPd > 000){
  ATRASSUAVEI();
  analogWrite(PWMA,255);
  analogWrite(PWMB,255);
  }
  }

if( ultrafront < 40){
  DERECHAPOCA();
  digitalWrite(LED1, HIGH);
  analogWrite(PWMA,255);
  analogWrite(PWMB,255);}

 if(LSH1 < 000){
  IZQUIERDAFUERTE();
   digitalWrite(LED1, HIGH);
  analogWrite(PWMA,255);
  analogWrite(PWMB,255);}
if (LSH2 < 000){
  DERECHAFUERTE();
   digitalWrite(LED1, HIGH);
  analogWrite(PWMA,255);
  analogWrite(PWMB,255);}
else{
  _BUSCAR();
  analogWrite(PWMA,255);
  analogWrite(PWMB,255);}
}
}

void adelante(){
  digitalWrite(AIN2,HIGH);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}

void _BUSCAR(){
  digitalWrite(AIN2,HIGH);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}
void DERECHAPOCA(){
   digitalWrite(AIN2,LOW);
  digitalWrite(AIN1,HIGH);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
  delay(400);
}
void IZQUIERDAPOCA(){
   digitalWrite(AIN2,HIGH);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
  delay(400);
}
void IZQUIERDAFUERTE(){
   digitalWrite(AIN2,HIGH);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
  delay(1000);
}
void DERECHAFUERTE(){
   digitalWrite(AIN2,LOW);
  digitalWrite(AIN1,HIGH);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
  delay(1000);
}
void ATRASSUAVEI(){
   digitalWrite(AIN2,HIGH);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
  delay(500);
}
void ATRASSUAVED(){
   digitalWrite(AIN2,HIGH);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
  delay(500);
}
else {
  digitalWrite(STMBY, LOW);}
}
