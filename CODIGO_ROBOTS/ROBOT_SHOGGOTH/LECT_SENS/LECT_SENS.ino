

#define SEN_L_L 18 //SENSOR LATERAL IZQUIERDO - ENTRADA A0
#define SEN_D_L 16 //SENSOR DIAGONAL IZQUIERDO - ENTRADA A1
#define SEN_F 15 //SENSOR FORNTAL - ENTRADA A2
#define SEN_D_R 14 //SENSOR DIAGONAL DERECHO - ENTRADA A3
#define SEN_L_R 11 //SENSOR LATERAL DERECHO - ENTRADA A4
#define SEN_P_L A3 //SENSOR DE PISO IZQUIERDO - ENTRADA A1
#define SEN_P_R A7 //SENSOR DE PISO DERECHO - ENTRADA A2
#define LT_L digitalRead(SEN_L_L) //LATERAL IZQUIERDO
#define LT_R digitalRead(SEN_L_R) //LATERAL DERECHO
#define FR digitalRead(SEN_F) //FRONTAL
#define DG_L digitalRead(SEN_D_L) //DIAGONAL IZQUIERDO
#define DG_R digitalRead(SEN_D_R) //DIAGONAL DERCHO
#define P_L analogRead(SEN_P_L) //PISO IZQUIERDO
#define P_R analogRead(SEN_P_R) //PISO DERECHO


void setup()
{
    pinMode(11, INPUT);
    pinMode(14, INPUT);
    pinMode(15, INPUT);
    pinMode(16, INPUT);
    pinMode(18, INPUT);

    Serial.begin(9600);
}

void loop()
{
    Serial.print(LT_L);
    Serial.print("\t");
    Serial.print(DG_L);
    Serial.print("\t");
    Serial.print(FR);
    Serial.print("\t");
    Serial.print(DG_R);
    Serial.print("\t");
    Serial.print(LT_R);
    Serial.print("\t");
    Serial.print(P_L);
    Serial.print("\t");
    Serial.println(P_R);

    delay(10);
}