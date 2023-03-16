/*
 * File:   main_P3_LCD_V2.c
 * Authors: Luis Castillo and Ian Fernandez
 *
 * Created on 16 de febrero de 2023, 15:55
 */

#include "p24HJ256GP610A.h"

#include "oscilator.h"
#include "GPIO.h"
#include "timers.h"
#include "commons.h"
#include "commons_P4.h"
#include "LCD.h"
#include "memoria.h"
#include "utilidades.h"
#include "UART2_RS232.h"
#include "ADC1.h"

// TCKPS para el prescaler

int main()
{
	inic_oscilator();	// Seleccón e inicialización del reloj: 80 MHz
	
	inic_leds();	// Inicialización leds: sentido y valor inicial.
    
    inic_pulsadores(); // Inicialización de pulsadores
    
    inic_LCD(); // Inicialización de la pantalla LCD
    
    copiar_FLASH_RAM(Mens_LCD_1, 0);
    copiar_FLASH_RAM(Mens_LCD_2, 1);
    
    line_1();    
    puts_lcd(Ventana_LCD[0], 16);
    line_2();
    puts_lcd(Ventana_LCD[1], 16);
    
    while(PORTDbits.RD6){
        
    }
    
    copiar_FLASH_RAM(Mens_LCD_3, 0);
    copiar_FLASH_RAM(Mens_LCD_4, 1);
    
    line_1();    
    puts_lcd(Ventana_LCD[0], 16);
    line_2();
    puts_lcd(Ventana_LCD[1], 16);
    
    while(PORTDbits.RD13){
        
    }
    
    copiar_FLASH_RAM(Mens_LCD_5, 0);
    copiar_FLASH_RAM(Mens_LCD_6, 1);
    
    line_1();    
    puts_lcd(Ventana_LCD[0], 16);
    line_2();
    puts_lcd(Ventana_LCD[1], 16);
    
    inic_crono();	// Inicialización variables cronometro.

	inic_Timer7();	// Inicialización T7 con un periodo de 10 milisegundos.
    inic_Timer5(); // Inicialización T5 con un periodo de 2.5 milisegundos.
    
    inic_CN(); // Inicialización de las interrupciones para los pulsadores.
    
    inic_UART2(); // Inicialización del modulo UART2
    
    U2TXREG = 0; // Envio de bit nulo atraves del modulo UART2
    
    inic_ADC1();
    comienzo_muestreo();
    
    
	while(1) {
        crono();     
        comprobar_inic_crono();

        if (flag_muestras == 1) {
            calcularMediaMuestras();
        }
	}
    
	return (0);
}