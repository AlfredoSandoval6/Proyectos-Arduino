/*
 * Button_Manager.c
 *
 *  Created on: 19 jul. 2019
 *      Author: Dex
 */

/* Inclusión de Librerías */
#include "Button_Manager.h"

/* Variables Globales */

/* Definición de Funciones */

/* ****************************************************************************************
 * Función: Digital_Input_Config
 * Argumentos: uint8_t indice_boton
 * Retorno: void
 * Descripción: Configura uno de los puertos destinados a manejar botones para que sea
 * 				entrada con resistencia interna en Pull Up.
 * Aplicación: Uso interno en la librería.
 * ****************************************************************************************/
void Digital_Input_Config(uint8_t indice_boton)
{
	switch(indice_boton)
	{
	case 7:
		DDR_BUTTON7 &= ~(1 << BUTTON7);
		PORT_BUTTON7 |= (1 << BUTTON7);
		break;
	case 6:
		DDR_BUTTON6 &= ~(1 << BUTTON6);
		PORT_BUTTON6 |= (1 << BUTTON6);
		break;
	case 5:
		DDR_BUTTON5 &= ~(1 << BUTTON5);
		PORT_BUTTON5 |= (1 << BUTTON5);
		break;
	case 4:
		DDR_BUTTON4 &= ~(1 << BUTTON4);
		PORT_BUTTON4 |= (1 << BUTTON4);
		break;
	case 3:
		DDR_BUTTON3 &= ~(1 << BUTTON3);
		PORT_BUTTON3 |= (1 << BUTTON3);
		break;
	case 2:
		DDR_BUTTON2 &= ~(1 << BUTTON2);
		PORT_BUTTON2 |= (1 << BUTTON2);
		break;
	case 1:
		DDR_BUTTON1 &= ~(1 << BUTTON1);
		PORT_BUTTON1 |= (1 << BUTTON1);
		break;
	case 0:
		DDR_BUTTON0 &= ~(1 << BUTTON0);
		PORT_BUTTON0 |= (1 << BUTTON0);
		break;
	default:
		break;
	}
}

/* ****************************************************************************
 * Función: Button_Reader_Starter
 * Argumentos: void
 * Retorno: void
 * Descripción: Configura los pines destinados a manejar botones para que sean
 * 				entradas con resistencia interna en Pull Up.
 * Nota: Se asume que los puertos tienen 8 bits.
 * Aplicación: Inicialización del main.
 * ****************************************************************************/
void Button_Reader_Starter(void)
{
	switch(N_BUTTONS)
	{
	case 8:
		Digital_Input_Config(7);
		Digital_Input_Config(6);
		Digital_Input_Config(5);
		Digital_Input_Config(4);
		Digital_Input_Config(3);
		Digital_Input_Config(2);
		Digital_Input_Config(1);
		Digital_Input_Config(0);
		break;
	case 7:
		Digital_Input_Config(6);
		Digital_Input_Config(5);
		Digital_Input_Config(4);
		Digital_Input_Config(3);
		Digital_Input_Config(2);
		Digital_Input_Config(1);
		Digital_Input_Config(0);
		break;
	case 6:
		Digital_Input_Config(5);
		Digital_Input_Config(4);
		Digital_Input_Config(3);
		Digital_Input_Config(2);
		Digital_Input_Config(1);
		Digital_Input_Config(0);
		break;
	case 5:
		Digital_Input_Config(4);
		Digital_Input_Config(3);
		Digital_Input_Config(2);
		Digital_Input_Config(1);
		Digital_Input_Config(0);
		break;
	case 4:
		Digital_Input_Config(3);
		Digital_Input_Config(2);
		Digital_Input_Config(1);
		Digital_Input_Config(0);
		break;
	case 3:
		Digital_Input_Config(2);
		Digital_Input_Config(1);
		Digital_Input_Config(0);
		break;
	case 2:
		Digital_Input_Config(1);
		Digital_Input_Config(0);
		break;
	case 1:
		Digital_Input_Config(0);
		break;
	default:
		break;
	}
}

/* ****************************************************************************
 * Función: Button0_Press_Event
 * Argumentos: void
 * Retorno: uint8_t
 * Descripción: Realiza una lectura de un flanco de bajada en el pin destinado
 * 				a BUTTON0.
 * Nota: Se asume que hay una resistencia interna Pull Up.
 * Aplicación: Uso cíclico del main.
 * ****************************************************************************/
uint8_t Button0_Press_Event(void)
{
	static uint8_t estado_anterior = 1;
	uint8_t estado_actual;
	uint8_t output = 0;

	estado_actual = (!!(PIN_BUTTON0 & (1 << BUTTON0)));

	if(!estado_actual && estado_anterior)
	{
		output = 1;
	}

	estado_anterior = estado_actual;

	return output;
}

/* ****************************************************************************
 * Función: Button1_Press_Event
 * Argumentos: void
 * Retorno: uint8_t
 * Descripción: Realiza una lectura de un flanco de bajada en el pin destinado
 * 				a BUTTON1.
 * Nota: Se asume que hay una resistencia interna Pull Up.
 * Aplicación: Uso cíclico del main.
 * ****************************************************************************/
uint8_t Button1_Press_Event(void)
{
	static uint8_t estado_anterior = 1;
	uint8_t estado_actual;
	uint8_t output = 0;

	estado_actual = (!!(PIN_BUTTON1 & (1 << BUTTON1)));

	if(!estado_actual && estado_anterior)
	{
		output = 1;
	}

	estado_anterior = estado_actual;

	return output;
}

/* ****************************************************************************
 * Función: Button2_Press_Event
 * Argumentos: void
 * Retorno: uint8_t
 * Descripción: Realiza una lectura de un flanco de bajada en el pin destinado
 * 				a BUTTON2.
 * Nota: Se asume que hay una resistencia interna Pull Up.
 * Aplicación: Uso cíclico del main.
 * ****************************************************************************/
uint8_t Button2_Press_Event(void)
{
	static uint8_t estado_anterior = 1;
	uint8_t estado_actual;
	uint8_t output = 0;

	estado_actual = (!!(PIN_BUTTON2 & (1 << BUTTON2)));

	if(!estado_actual && estado_anterior)
	{
		output = 1;
	}

	estado_anterior = estado_actual;

	return output;
}

/* ****************************************************************************
 * Función: Button3_Press_Event
 * Argumentos: void
 * Retorno: uint8_t
 * Descripción: Realiza una lectura de un flanco de bajada en el pin destinado
 * 				a BUTTON3.
 * Nota: Se asume que hay una resistencia interna Pull Up.
 * Aplicación: Uso cíclico del main.
 * ****************************************************************************/
uint8_t Button3_Press_Event(void)
{
	static uint8_t estado_anterior = 1;
	uint8_t estado_actual;
	uint8_t output = 0;

	estado_actual = (!!(PIN_BUTTON3 & (1 << BUTTON3)));

	if(!estado_actual && estado_anterior)
	{
		output = 1;
	}

	estado_anterior = estado_actual;

	return output;
}

/* ****************************************************************************
 * Función: Button4_Press_Event
 * Argumentos: void
 * Retorno: uint8_t
 * Descripción: Realiza una lectura de un flanco de bajada en el pin destinado
 * 				a BUTTON4.
 * Nota: Se asume que hay una resistencia interna Pull Up.
 * Aplicación: Uso cíclico del main.
 * ****************************************************************************/
uint8_t Button4_Press_Event(void)
{
	static uint8_t estado_anterior = 1;
	uint8_t estado_actual;
	uint8_t output = 0;

	estado_actual = (!!(PIN_BUTTON4 & (1 << BUTTON4)));

	if(!estado_actual && estado_anterior)
	{
		output = 1;
	}

	estado_anterior = estado_actual;

	return output;
}

/* ****************************************************************************
 * Función: Button5_Press_Event
 * Argumentos: void
 * Retorno: uint8_t
 * Descripción: Realiza una lectura de un flanco de bajada en el pin destinado
 * 				a BUTTON5.
 * Nota: Se asume que hay una resistencia interna Pull Up.
 * Aplicación: Uso cíclico del main.
 * ****************************************************************************/
uint8_t Button5_Press_Event(void)
{
	static uint8_t estado_anterior = 1;
	uint8_t estado_actual;
	uint8_t output = 0;

	estado_actual = (!!(PIN_BUTTON5 & (1 << BUTTON5)));

	if(!estado_actual && estado_anterior)
	{
		output = 1;
	}

	estado_anterior = estado_actual;

	return output;
}

/* ****************************************************************************
 * Función: Button6_Press_Event
 * Argumentos: void
 * Retorno: uint8_t
 * Descripción: Realiza una lectura de un flanco de bajada en el pin destinado
 * 				a BUTTON6.
 * Nota: Se asume que hay una resistencia interna Pull Up.
 * Aplicación: Uso cíclico del main.
 * ****************************************************************************/
uint8_t Button6_Press_Event(void)
{
	static uint8_t estado_anterior = 1;
	uint8_t estado_actual;
	uint8_t output = 0;

	estado_actual = (!!(PIN_BUTTON6 & (1 << BUTTON6)));

	if(!estado_actual && estado_anterior)
	{
		output = 1;
	}

	estado_anterior = estado_actual;

	return output;
}

/* ****************************************************************************
 * Función: Button7_Press_Event
 * Argumentos: void
 * Retorno: uint8_t
 * Descripción: Realiza una lectura de un flanco de bajada en el pin destinado
 * 				a BUTTON7.
 * Nota: Se asume que hay una resistencia interna Pull Up.
 * Aplicación: Uso cíclico del main.
 * ****************************************************************************/
uint8_t Button7_Press_Event(void)
{
	static uint8_t estado_anterior = 1;
	uint8_t estado_actual;
	uint8_t output = 0;

	estado_actual = (!!(PIN_BUTTON7 & (1 << BUTTON7)));

	if(!estado_actual && estado_anterior)
	{
		output = 1;
	}

	estado_anterior = estado_actual;

	return output;
}

