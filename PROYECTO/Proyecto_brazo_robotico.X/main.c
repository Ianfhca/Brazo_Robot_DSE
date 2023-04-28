/*
 * Fichero:   main.c
 * Autores: Luis Castillo and Ian Fernandez
 * Descricion: fichero Main del Proyecto
 * 
 */

#include "p24HJ256GP610A.h"

#include "oscilator.h"
#include "GPIO.h"
#include "timers.h"
#include "commons.h"
#include "LCD.h"
#include "memoria.h"
#include "UART2_RS232.h"
#include "utilidades.h"

int main(void) {
    
    inic_oscilator();	// Seleccion e inicializaci?n del reloj: 80 MHz
	
	inic_leds();	// Inicializacion leds: sentido y valor inicial.
    
    inic_pulsadores(); // Inicializacion de pulsadores
    
    inic_LCD(); // Inicializacion de la pantalla LCD
    
    line_1();    
    puts_lcd(Ventana_LCD[0], 16);
    line_2();
    puts_lcd(Ventana_LCD[1], 16);
    
    while(PORTDbits.RD6){
        
    }
    
    scrollLCD(0);
    
    inic_crono();	// Inicializacion variables cronometro.

	inic_Timer7();	// Inicializacion T7 con un periodo de 10 milisegundos.

    inic_Timer5(); // Inicializacion T5 con un periodo de 2.5 milisegundos.
    
    inic_CN(); // Inicializacion de las interrupciones para los pulsadores.
    
    inic_UART2();
    U2TXREG = 0;
    
    inic_ADC1();
    
    while(1){
        crono();     
        comprobar_inic_crono();
        
        if(flagScroll==0){
            scrollLCD(0);
            flagScroll = -1;
        }
        else if(flagScroll==1){
            scrollLCD(1);
            flagScroll = -1;
        }
            
            
        if(indice==3 || indice==4)
            actualizarLCD();
    }
    
    return 0;
}
