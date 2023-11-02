/*PROGRAMA 1:
#define ledL 13

void setup() {

  Serial.begin(9600);
  pinMode(ledL, OUTPUT);
  
}

byte i = 0;

void loop() {
  
  while(i < 3){
    digitalWrite(ledL, HIGH);
    delay(100);
    digitalWrite(ledL, LOW);
    delay(100);
    ++i;
  
  }

  i = 0;
  
  delay(500);
  Serial.println(millis());

}
*/

/* PROGRAMA 2
void setup(){

  Serial.begin(9600);
  Serial.println("Escribe un valor.");

}

byte entrada;
byte numByt;

void loop(){

  if((numByt = Serial.available()) > 0){

    Serial.println("");
    Serial.println("Tamaño de buffer: ");
    Serial.print(numByt);
    Serial.println("");
    Serial.println("El valor es:");
    
    while(numByt > 0){

        entrada = Serial.read();
        Serial.write(entrada);
        --numByt;
      
      }
    
    }

}
*/

//PROGRAMA 3:
void setup(){

  
  
  }

void loop(){

  
  
  }
