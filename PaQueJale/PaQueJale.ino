
#define pin2 1
#define pin3 0

void setup() {
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);


}

void loop() {
   digitalWrite(pin2, HIGH);
    digitalWrite(pin3, LOW);
    delay(250);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, HIGH);
    delay(250);

}
