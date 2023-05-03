/*
 * Fichero: GPIO.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Inicializacion y tratamiento de pulsadores y leds
 * 
 */

// Definiciones necesarias
#include "p24HJ256GP610A.h"
#include "commons.h"

void inic_leds() {
    TRISA = 0xff00;       // Definir como salidas los 8 pines conectados a 
                          // leds: RA7-RA. El resto como entradas: 1 = In; 0 = Out
    LATA=LATA & 0xff00;   // Apagar los leds
    TRISDbits.TRISD0 = 0; // Servomotor ESTO NO VA AQUI
}

void inic_pulsadores(){
    AD1PCFGH = 0xffff;    // Todos los pulsadores a 0
  
    TRISDbits.TRISD13 = 1;// Pulsador S4
    TRISDbits.TRISD6 = 1; // Pulsador S3
    TRISAbits.TRISA7 = 1; // Pulsador S5 (Cambia de modo)
}
      
