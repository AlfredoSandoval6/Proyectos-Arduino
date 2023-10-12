//PRUEBA DE SUMA DE BITS

void setup() {
  
  Serial.begin(9600);

}

byte n1 = 0B00000010, n2 = 0B00000001, n3 = 0B00000100, res = 0B00000000;

void loop() {
  
  res = n1 + n2 + n3;

  Serial.println(res, BIN);
  delay(1000);

}

//PRUEBA EXITOSA: LA SUMA DE BITS DIÓ COMO RESULTADO (res = )0B00000111
