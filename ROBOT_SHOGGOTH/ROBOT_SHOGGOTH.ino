//ROBOT SHOGGOTH

////CONSTANTES
#define RAN_SEN 250 //RANGO DE LOS SENSORES FRONTALES
#define RAN_SEN_P 880 //RANGO DE LOS SENSORES DE PISO
#define NUM_SEN 5 //NÚMERO DE SENSORES FRONTALES
#define NUM_SEN_P 2 //NÚMERO DE SENSORES DE PISO

////ENTRADAS
/*#define SEN_L_L A0 //SENSOR LATERAL IZQUIERDO - ENTRADA A0
#define SEN_D_L A1 //SENSOR DIAGONAL IZQUIERDO - ENTRADA A1
#define SEN_F A2 //SENSOR FORNTAL - ENTRADA A2
#define SEN_D_R A3 //SENSOR DIAGONAL DERECHO - ENTRADA A3
#define SEN_L_R A4 //SENSOR LATERAL DERECHO - ENTRADA A4*/
#define SEN_P_L A1 //SENSOR DE PISO IZQUIERDO - ENTRADA A1
#define SEN_P_R A2 //SENSOR DE PISO DERECHO - ENTRADA A2
#define ACTIVADOR 12 //NÚMERO DE PIN ESTABLECIDO PARA EL ACTIVADOR

////SALIDAS - MOTORES <1 SENTIDO HORARIO - 0 SENTIDO ANTIHORARIO>
#define DIRL 4 //MOTOR IZQUIERDO
#define PWML 6 //PWM IZQUIERDO
#define DIRR 3 //MOTOR DERECHO
#define PWMR 5 //PWM DERECHO

////VARIABLES
//SENSORES
/*#define LT_L analogRead(SEN_L_L) //LATERAL IZQUIERDO
#define LT_R analogRead(SEN_L_R) //LATERAL DERECHO
#define FR analogRead(SEN_F) //FRONTAL
#define DG_L analogRead(SEN_D_L) //DIAGONAL IZQUIERDO
#define DG_R analogRead(SEN_D_R) //DIAGONAL DERCHO*/
#define P_L analogRead(SEN_P_L) //PISO IZQUIERDO
#define P_R analogRead(SEN_P_R) //PISO DERECHO
#define LECT_ACT(state) (digitalRead(ACTIVADOR) == (state)) //LECTURA DEL ACTIVADOR, DA 0 O 1 ACUERDO AL ESTADO
int estrategia[2] = {2, 7}; //PINES DE ESTRATEGIA
int sgy_leds[2] = {0, 1}; //PINES DE LEDS DE ESTRATEGIA
int der = false, izq = false; //ÚLTIMA POSICIÓN REGISTRADA, DE IZQUIERDA Ó DERECHA
byte sgy; //ALMACENAMIENTO DE LA ESTRATEGIA SELECCIONADA

//LECTURA DE SENSORES
byte sensores; //0B00000000

//ITERADORES
byte i, j, k;

////PROTOTIPO DE FUNCIONES
void secuenciaLeds(); //SECUENCIA DE LEDS POR ESTRATEGIA CON INTERRUPCIÓN DE ACTIVADOR
void espera(bool caso, int T); //MÉTODO DE DELAY CON INTERRUPCIÓN DE ACTIVADOR Y/O SENSORES
void lectura(); //LECTURA DE SENSORES
void rutina(); //ESTRATEGIA A UTILIZAR EN EL ARRANQUE, Y LUEGO BUSCAR AL CONTRINCANTE
void posAnalisis(); //ANALISAR LA POSICIÓN DEL CONTRINCANTE PARA SABER CÓMO Y A DÓNDE MOVERSE
void busqueda(); //BÚSQUEDA DEL CONTRINCANTE SI NO SE DETECTA EN LOS SENSORES FRONTALES
void attack(); //MÉTODO DE ATAQUE CUANDO SE TENGA AL CONTRINCANTE EN LOS SENSORES FRONTALES
void paro(int T); //FUNCIÓN (MÉTODO) PARA EL PARO
void izquierda(int PWMI, int PWMD, int T); //GIRO A LA IZQUIERDA
void derecha(int PWMI, int PWMD, int T); //GIRO A LA DERECHA
void adelante(int PWMI, int PWMD, int T); //AVANZAR HACIA ADELANTE
void atras(int T); //RETROCESO

void setup() {

  //MOTORES DECLARADOS COMO SALIDAS Y CON bAJA TENSIÓN
  pinMode(DIRL, OUTPUT);
  pinMode(DIRR, OUTPUT);
  paro(1);

  //PIN DEL ACTIVADOR DECLARADO COMO ENTRADA
  pinMode(ACTIVADOR, INPUT);

  //PINES DE ESTRATEGIA DECLARADOS COMO ENTRADAS
  pinMode(estrategia[0], INPUT);
  pinMode(estrategia[1], INPUT);

  //PINES DE LEDS DE ESTRATEGIA DECLARADOS COMO SALIDAS
  pinMode(sgy_leds[0], OUTPUT);
  pinMode(sgy_leds[1], OUTPUT);

  //INICIALIZAR VALORES DE VARIABLES EN 0
  i = j = k = sgy = sensores = 0;

  lectura();
  delay(3000);

}

void loop() {

  do{
    secuenciaLeds();
  }while(LECT_ACT(LOW));

  delay(1);
  if(LECT_ACT(HIGH)){i = j = 0; rutina();}

}

void secuenciaLeds(){

  i = digitalRead(estrategia[0]);
  j = digitalRead(estrategia[1]);

  if((i == LOW) && (j == LOW)){
    digitalWrite(sgy_leds[0], HIGH); digitalWrite(sgy_leds[1], HIGH);
    for(int l = 0; l <= 500; ++l){if(LECT_ACT(HIGH)) return; delay(1);}
    digitalWrite(sgy_leds[0], LOW); digitalWrite(sgy_leds[1], LOW);
    for(int l = 0; l <= 500; ++l){if(LECT_ACT(HIGH)) return; delay(1);}
    sgy = 0B00000001;
  } else if((i == LOW) && (j == HIGH)){
    digitalWrite(sgy_leds[0], HIGH); digitalWrite(sgy_leds[1], LOW);
    for(int l = 0; l <= 400; ++l){if(LECT_ACT(HIGH)) return; delay(1);}
    digitalWrite(sgy_leds[0], LOW); digitalWrite(sgy_leds[1], HIGH);
    for(int l = 0; l <= 400; ++l){if(LECT_ACT(HIGH)) return; delay(1);}
    sgy = 0B00000010;
  } else if((i == HIGH) && (j == LOW)){
    digitalWrite(sgy_leds[0], HIGH); digitalWrite(sgy_leds[1], LOW);
    for(int l = 0; l <= 300; ++l){if(LECT_ACT(HIGH)) return; delay(1);}
    digitalWrite(sgy_leds[0], HIGH); digitalWrite(sgy_leds[1], HIGH);
    for(int l = 0; l <= 300; ++l){if(LECT_ACT(HIGH)) return; delay(1);}
    digitalWrite(sgy_leds[0], LOW); digitalWrite(sgy_leds[1], LOW);
    for(int l = 0; l <= 300; ++l){if(LECT_ACT(HIGH)) return; delay(1);}
    sgy = 0B00000100;
  } else if((i == HIGH) && (j == HIGH)){
    digitalWrite(sgy_leds[0], HIGH); digitalWrite(sgy_leds[1], HIGH);
    for(int l = 0; l <= 250; ++l){if(LECT_ACT(HIGH)) return; delay(1);}
    digitalWrite(sgy_leds[0], LOW); digitalWrite(sgy_leds[1], LOW);
    for(int l = 0; l <= 250; ++l){if(LECT_ACT(HIGH)) return; delay(1);}
    sgy = 0B00001000;
  }

}

void espera(bool caso, int T){
  if(caso){
    for(int l = 0; l<= T; ++l){
      if(LECT_ACT(LOW)){paro(1); return;} delay(1);
    }
  } else{
    for(int l = 0; l<= T; ++l){
      lectura(); if(LECT_ACT(LOW)){paro(1); return;} if(sensores != 0) return; delay(1);
    }
  }
}

void lectura(){

  /*if(LT_L > RAN_SEN){sensores |= (1 << 0);} else{sensores &= ~ (1 << 0);}
  if(LT_R > RAN_SEN){sensores |= (1 << 1);} else{sensores &= ~ (1 << 1);}
  if(FR > RAN_SEN){sensores |= (1 << 2);} else{sensores &= ~ (1 << 2);}
  if(DG_L > RAN_SEN){sensores |= (1 << 3);} else{sensores &= ~ (1 << 3);}
  if(DG_R > RAN_SEN){sensores |= (1 << 4);} else{sensores &= ~ (1 << 4);}*/
  if(P_L < RAN_SEN_P){sensores |= (1 << 5);} else{sensores &= ~ (1 << 5);}
  if(P_R < RAN_SEN_P){sensores |= (1 << 6);} else{sensores &= ~ (1 << 6);}

  if(bitRead(sensores, 0) && !bitRead(sensores, 4)) izq = true;
  else if(!bitRead(sensores, 0) && bitRead(sensores, 4)) der = true;

}

void rutina(){

  switch (sgy){
    case 0B00000001: //GIRO DERECHA A 90°
      derecha(240, 240, 60);
    break;

    case 0B00000010: //GIRO DERECHA A 180°
      derecha(240, 237, 120);
    break;

    case 0B00000100: //GIRO IZQUIERDA A 90°
      izquierda(240, 240, 60);
    break;

    case 0B00001000: //SIN RUTINA, EMPEZAR BÚSQUEDA
      adelante(240, 240, 60);
    break;

    default:
    break;
  }

  do{
    posAnalisis();
  }while(LECT_ACT(HIGH));

}

void posAnalisis(){

  lectura();

  if(sensores == 0) busqueda();
  if(LECT_ACT(LOW)) return;

  if(bitRead(sensores, 5) || bitRead(sensores, 6)){

      if(bitRead(sensores, 5) && bitRead(sensores, 6)){
        atras(100); izquierda(240, 240, 125);
      } else{

      if(bitRead(sensores, 5)){
        atras(100); izquierda(240, 240, 125);
      }

      if(bitRead(sensores, 6)){
        atras(100); derecha(240, 240, 125);
      }

    }

    paro(1);
    espera(false, 249);

  } else{/*

    if(bitRead(sensores, 0)){
      if(bitRead(sensores, 1)){
        if(bitRead(sensores, 2)){
          if(bitRead(sensores, 3)){
            if(bitRead(sensores, 4)){
              adelante(250, 250, 20); attack(); return;
            } else{izquierda(); return;}
          } else{izquierda(); return;}
        } else{izquierda(); return;}
      } else{ izquierda(); return;}
    }

    if(bitRead(sensores, 4)){
      if(bitRead(sensores, 3)){
        if(bitRead(sensores, 2)){
          if(bitRead(sensores, 1)){
            derecha(); return;
          } else{derecha(); return;}
        } else{derecha(); return;}
      } else{derecha(); return;}
    }

    switch(sensores){

      case 0B00000010:
        izquierda(); return;
        break;

      case 0B00001000:
        derecha(); return;
        break;

    }

    if(bitRead(sensores, 2)){
      if(bitRead(sensores, 1)){
        if(bitRead(sensores, 3)){
          adelante(250, 250, 10); attack(); return;
        } else if(bitRead(sensores, 0)){izquierda(); return;} else{izquierda(); return;}
      } else if(bitRead(sensores, 3)){derecha(); return;} else{adelante(); attack(); return;}
    }

    switch (sensores){
    
    case 0B00001110:
      adelante(250, 250, 50);
      attack();
      break;
    
    case 0B00011111:
      adelante(250, 250, 50);
      attack();
      break;

    case 0B00000100:
      adelante(250, 250, 50);
      attack();
      break;
    
    default:
      break;
    }*/

  }

}

void busqueda(){
  if(izq || der){
    if(LECT_ACT(LOW)) return;
    if(izq){
    digitalWrite(DIRL, LOW); analogWrite(PWML, 240);
    digitalWrite(DIRR, HIGH); analogWrite(PWMR, 240);
    espera(false, 120);
    }
    if(der){
    digitalWrite(DIRL, HIGH); analogWrite(PWML, 240);
    digitalWrite(DIRR, LOW); analogWrite(PWMR, 240);
    espera(false, 120);
    }
    izq = der = false;
  }
  if(sensores == 0){
    do{
      if(LECT_ACT(LOW)) break;
      adelante(24, 30, 2);
      lectura();
    }while(sensores == 0);
  }
}

void attack(){

//ATAQUE CUANDO LOS SENSORES FRONTALES DETECTEN

  while(1){

    lectura();

    switch(sensores){
      
      case 0B00001110:
        adelante(250, 250, 50);
      break;

      case 0B00000100:
        adelante(250, 250, 50);
      break;

      case 0B00011111:
        adelante(250, 250, 50);
      break;

      default:
        j = 1;
      break;

    }

    if(j){j = 0; break;}

  }

}

void paro(int T){

  digitalWrite(DIRL, LOW);
  analogWrite(PWML, 0);
  digitalWrite(DIRR, LOW);
  analogWrite(PWMR, 0);
  delay(T);

}

void izquierda(int PWMI, int PWMD, int T){

  digitalWrite(DIRL, LOW);
  analogWrite(PWML, PWMI);
  digitalWrite(DIRR, HIGH);
  analogWrite(PWMR, PWMD);
  espera(true, T);
  
}

void derecha(int PWMI, int PWMD, int T){

  digitalWrite(DIRL, HIGH);
  analogWrite(PWML, PWMI);
  digitalWrite(DIRR, LOW);
  analogWrite(PWMR, PWMD);
  espera(true, T);
  
}

void adelante(int PWMI, int PWMD, int T){

  digitalWrite(DIRL, HIGH);
  analogWrite(PWML, PWMI);
  digitalWrite(DIRR, HIGH);
  analogWrite(PWMR, PWMD);
  espera(true, T);

}

void atras(int T){

  digitalWrite(DIRL, LOW);
  analogWrite(PWML, 255);
  digitalWrite(DIRR, LOW);
  analogWrite(PWMR, 255);
  espera(true, T);
  
}
