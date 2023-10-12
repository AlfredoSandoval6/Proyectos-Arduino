#define SUMA(value1, value2) ((value1) + (value2))
#define MULTIPLICACION(value1, value2) (value1 * value2) //ERRÓNEO
#define MULTIBIEN(value1, value2) ((value1) * (value2)) //CORRECTO

void setup(){

    Serial.begin(9600);

}

void loop(){

    Serial.println(SUMA(12+1, 3));
    delay(500);
    Serial.println(MULTIPLICACION(12+1, 3));
    delay(500);
    Serial.println(MULTIBIEN(12+1, 3));
    delay(2500);

}

//EL EXPERIMENTO HA DEMOSTRADO QUE "ENCERRAR" APROPIADAMENTE LOS VALORES DENTRO DE LAS MACTROS ES VITAL;
//ADEMÁS, SE EXPERIMENTÓ CON LA DEFINICIÓN Y USO DE LAS MACROS, CON UN CORRECTO "ENGLOBADO" Y
//ESTABLECIMIENTO DE VARIABLES Y OPERADORES.