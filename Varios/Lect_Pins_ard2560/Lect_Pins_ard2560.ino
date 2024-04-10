void setup(){
    pinMode(1, INPUT); pinMode(2, INPUT);  pinMode(3, INPUT);  pinMode(4, INPUT);  pinMode(5, INPUT);
    pinMode(6, INPUT); pinMode(7, INPUT);  pinMode(8, INPUT);  pinMode(9, INPUT);  pinMode(10, INPUT);
    pinMode(11, INPUT); pinMode(12, INPUT);  pinMode(13, INPUT);  pinMode(14, INPUT);  pinMode(15, INPUT);
    pinMode(16, INPUT); pinMode(17, INPUT);  pinMode(18, INPUT);  pinMode(19, INPUT);  pinMode(20, INPUT);
    pinMode(21, INPUT); pinMode(22, INPUT);  pinMode(23, INPUT);  pinMode(24, INPUT);  pinMode(25, INPUT);
    pinMode(26, INPUT); pinMode(27, INPUT);  pinMode(28, INPUT);  pinMode(29, INPUT);  pinMode(20, INPUT);
    pinMode(31, INPUT); pinMode(32, INPUT);  pinMode(33, INPUT);  pinMode(34, INPUT);  pinMode(35, INPUT);
    pinMode(36, INPUT); pinMode(37, INPUT);  pinMode(38, INPUT);  pinMode(39, INPUT);  pinMode(30, INPUT);
    pinMode(41, INPUT); pinMode(42, INPUT);  pinMode(43, INPUT);  pinMode(44, INPUT);  pinMode(45, INPUT);
    pinMode(46, INPUT); pinMode(47, INPUT);  pinMode(48, INPUT);  pinMode(49, INPUT);  pinMode(50, INPUT);
    pinMode(51, INPUT); pinMode(52, INPUT);  pinMode(53, INPUT);
    Serial.begin(9600);
}

void loop(){
    Serial.print(digitalRead(1)); Serial.print(" "); Serial.print(digitalRead(2)); Serial.print(" ");
    Serial.print(digitalRead(3)); Serial.print(" "); Serial.print(digitalRead(4)); Serial.print(" ");
    Serial.print(digitalRead(5)); Serial.print(" "); Serial.print(digitalRead(6)); Serial.print(" ");
    Serial.print(digitalRead(7)); Serial.print(" "); Serial.print(digitalRead(8)); Serial.print(" ");
    Serial.print(digitalRead(9)); Serial.print(" "); Serial.print(digitalRead(10)); Serial.println(" ");
    Serial.print(digitalRead(11)); Serial.print(" "); Serial.print(digitalRead(12)); Serial.print(" ");
    Serial.print(digitalRead(13)); Serial.print(" "); Serial.print(digitalRead(14)); Serial.print(" ");
    Serial.print(digitalRead(15)); Serial.print(" "); Serial.print(digitalRead(16)); Serial.print(" ");
    Serial.print(digitalRead(17)); Serial.print(" "); Serial.print(digitalRead(18)); Serial.print(" ");
    Serial.print(digitalRead(19)); Serial.print(" "); Serial.print(digitalRead(20)); Serial.println(" ");
    Serial.print(digitalRead(21)); Serial.print(" "); Serial.print(digitalRead(22)); Serial.print(" ");
    Serial.print(digitalRead(23)); Serial.print(" "); Serial.print(digitalRead(24)); Serial.print(" ");
    Serial.print(digitalRead(25)); Serial.print(" "); Serial.print(digitalRead(26)); Serial.print(" ");
    Serial.print(digitalRead(27)); Serial.print(" "); Serial.print(digitalRead(28)); Serial.print(" ");
    Serial.print(digitalRead(29)); Serial.print(" "); Serial.print(digitalRead(30)); Serial.println(" ");
    Serial.print(digitalRead(31)); Serial.print(" "); Serial.print(digitalRead(32)); Serial.print(" ");
    Serial.print(digitalRead(33)); Serial.print(" "); Serial.print(digitalRead(34)); Serial.print(" ");
    Serial.print(digitalRead(35)); Serial.print(" "); Serial.print(digitalRead(36)); Serial.print(" ");
    Serial.print(digitalRead(37)); Serial.print(" "); Serial.print(digitalRead(38)); Serial.print(" ");
    Serial.print(digitalRead(39)); Serial.print(" "); Serial.print(digitalRead(40)); Serial.println(" ");
    Serial.print(digitalRead(41)); Serial.print(" "); Serial.print(digitalRead(42)); Serial.print(" ");
    Serial.print(digitalRead(43)); Serial.print(" "); Serial.print(digitalRead(44)); Serial.print(" ");
    Serial.print(digitalRead(45)); Serial.print(" "); Serial.print(digitalRead(46)); Serial.print(" ");
    Serial.print(digitalRead(47)); Serial.print(" "); Serial.print(digitalRead(48)); Serial.print(" ");
    Serial.print(digitalRead(49)); Serial.print(" "); Serial.print(digitalRead(50)); Serial.println(" ");
    Serial.print(digitalRead(51)); Serial.print(" "); Serial.print(digitalRead(52)); Serial.print(" ");
    Serial.print(digitalRead(53)); Serial.println(" ");
    Serial.println("--------------------");
    delay(100);
}