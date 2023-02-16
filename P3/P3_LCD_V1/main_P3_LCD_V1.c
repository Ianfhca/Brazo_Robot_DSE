/*
 * File:   main_P3_LCD_V1.c
 * Author: Ian
 *
 * Created on 16 de febrero de 2023, 15:55
 */

#include "p24HJ256GP610A.h"

#include "oscilator.h"
#include "GPIO.h"
#include "timers.h"
#include "commons.h"
#include "LCD.h"
#include "memoria.h"

// TCKPS para el prescaler

int main()
{
    unsigned char msg[4] = "Hola";
	inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz
	
	inic_leds();	// Inicializacio³n leds: sentido y valor inicial.
    
    inic_pulsadores(); //Inicializacion de pulsadores
	
    inic_LCD();
    
    line_1();
    
    puts_lcd(msg, 4);
	//inic_crono();	//Inicializacion variables cronometro.

	//inic_Timer7();	// Inicializacion T7 con un periodo de 10 milisegundos.
    
    //inic_CN();
   
    
	while(1) {
        
	}
	return (0);
}
