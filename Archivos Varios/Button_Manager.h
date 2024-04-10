/*
 * Button_Manager.h
 *
 *  Created on: 19 jul. 2019
 *      Author: Dex
 */

#ifndef BUTTON_MANAGER_H_
#define BUTTON_MANAGER_H_

/* Inclusión de Librerías */
#include <stdint.h>
#include <avr/io.h>

/* Definicíon de Constantes y Macros */
#define N_BUTTONS		2

/* Botones */
#define BUTTON0			7
#define BUTTON1			6
#define BUTTON2			2
#define BUTTON3			3
#define BUTTON4			4
#define BUTTON5			5
#define BUTTON6			6
#define BUTTON7			7

/* Puertos de Lectura */
#define DDR_BUTTON0		DDRK
#define PORT_BUTTON0	PORTK
#define PIN_BUTTON0		PINK

#define DDR_BUTTON1		DDRK
#define PORT_BUTTON1	PORTK
#define PIN_BUTTON1		PINK

#define DDR_BUTTON2		DDRK
#define PORT_BUTTON2	PORTK
#define PIN_BUTTON2		PINK

#define DDR_BUTTON3		DDRK
#define PORT_BUTTON3	PORTK
#define PIN_BUTTON3		PINK

#define DDR_BUTTON4		DDRK
#define PORT_BUTTON4	PORTK
#define PIN_BUTTON4		PINK

#define DDR_BUTTON5		DDRK
#define PORT_BUTTON5	PORTK
#define PIN_BUTTON5		PINK

#define DDR_BUTTON6		DDRK
#define PORT_BUTTON6	PORTK
#define PIN_BUTTON6		PINK

#define DDR_BUTTON7		DDRK
#define PORT_BUTTON7	PORTK
#define PIN_BUTTON7		PINK

/* Errores de Configuración */
#if (N_BUTTONS > 8) || (N_BUTTONS < 1)
#error "N_BUTTONS debe ser entero en el rango de 1 y 8."
#endif

#if (BUTTON0 > 7) || (BUTTON0 < 0)
#error "BUTTON0 debe ser entero en el rango de 0 y 7."
#endif

#if (BUTTON1 > 7) || (BUTTON1 < 0)
#error "BUTTON1 debe ser entero en el rango de 0 y 7."
#endif

#if (BUTTON2 > 7) || (BUTTON2 < 0)
#error "BUTTON2 debe ser entero en el rango de 0 y 7."
#endif

#if (BUTTON3 > 7) || (BUTTON3 < 0)
#error "BUTTON3 debe ser entero en el rango de 0 y 7."
#endif

#if (BUTTON4 > 7) || (BUTTON4 < 0)
#error "BUTTON4 debe ser entero en el rango de 0 y 7."
#endif

#if (BUTTON5 > 7) || (BUTTON5 < 0)
#error "BUTTON5 debe ser entero en el rango de 0 y 7."
#endif

#if (BUTTON6 > 7) || (BUTTON6 < 0)
#error "BUTTON6 debe ser entero en el rango de 0 y 7."
#endif

#if (BUTTON7 > 7) || (BUTTON7 < 0)
#error "BUTTON7 debe ser entero en el rango de 0 y 7."
#endif

/* Definición de Tipos y Estructuras */

/* Variables Globales con extern */

/* Declaración de Funciones */
void Digital_Input_Config(uint8_t indice_boton);
void Button_Reader_Starter(void);
uint8_t Button0_Press_Event(void);
uint8_t Button1_Press_Event(void);
uint8_t Button2_Press_Event(void);
uint8_t Button3_Press_Event(void);
uint8_t Button4_Press_Event(void);
uint8_t Button5_Press_Event(void);
uint8_t Button6_Press_Event(void);
uint8_t Button7_Press_Event(void);

#endif /* BUTTON_MANAGER_H_ */
