/*
 * File:   main_P6_OCPWM_V2.c
 * Authors: Luis Castillo and Ian Fernandez
 *
 */

#include "p24HJ256GP610A.h"

#include "oscilator.h"
#include "GPIO.h"
#include "timers.h"
#include "commons.h"
#include "LCD.h"
#include "memoria.h"
#include "utilidades.h"
#include "UART2_RS232.h"
#include "ADC1.h"
#include "OCPWM.h"

int main()
{
	inic_oscilator();	// Seleccón e inicialización del reloj: 80 MHz
	
	inic_leds();	// Inicialización leds: sentido y valor inicial.
    
    inic_pulsadores(); // Inicialización de pulsadores
    
    inic_LCD(); // Inicialización de la pantalla LCD
    
    //mensajes iniciales LCD:
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
    
    inic_ADC1();  //Inicializacion del modulo ADC
    inic_Timer3(); // Inicialización T5 con un periodo de 1 milisegundos.
    
    
    //inic_OC1();
    mostrar_duty();
    inic_Timer2();
    
    InitI2C_1();
    inic_Timer6();
    
    unsigned char dist[2];
    unsigned char dirI2C = 0xE2;
    unsigned int aux = 0;
    
    inic_medicion_dis(dirI2C); 
    
	while(1) {
        crono();     
        comprobar_inic_crono();

        if (flag_muestras == 1) { //Se ha completado la recoleccion de las 8 muestras de cada dispositivo analogico
            calcular_media_muestras(); //Se calculan la media de las muestras y se despliegan en LCD
            
            flag_muestras = 0;
        }
        
        if(flag_T6){
            T6CONbits.TON = 0;
            TMR6 = 0;
            
            leer_medicion(dirI2C, dist);
            
            aux = conversion_BytesToInt(dist);
            
            conversion_4digitos(&Ventana_LCD[0][12], aux);
            
            inic_medicion_dis(dirI2C);
            
            T6CONbits.TON = 1;
            flag_T6 = 0;
        }
	}
    
	return (0);
}