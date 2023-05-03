/*
 * Fichero: GPIO.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Inicializacion y tratamiento de pulsadores y leds
 * 
 */

// Definiciones necesarias
#include "p24HJ256GP610A.h"
#include "commons.h"

void inic_leds()
{
  TRISA = 0xff00; // Definir como salidas los 8 pines conectados a leds: RA7-RA0		  // El resto como entradas: 1 = In; 0 = Out
  Nop();	
  Nop();
  LATA=LATA & 0xff00; 	// Apagar los leds
  TRISDbits.TRISD0 = 0; //Servomotor
}

void inic_pulsadores(){
    
  AD1PCFGH = 0xffff;
  
  TRISDbits.TRISD13 = 1; // Pulsador S4
  TRISDbits.TRISD6 = 1; // Pulsador S3
  TRISAbits.TRISA7 = 1; // Pulsador S5 (cambia de modo)
  
  
}
      
