/*
 * Fichero: CN.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Funciones relacionadas con la inicializacion del modulo CN
 * y su correspondiente rutina de atencion
 * 
 */

// Definiciones necesarias
#include "p24HJ256GP610A.h"
#include "commons.h"
#include "timers.h"
#include "memoria.h"
#include "PWM.h"

int flag_gatillo = 0, flag_ini = 0, flag_derecho = 0;

// Funcion para inicializar el modulo CN
void inic_CN()
{
  	CNEN1bits.CN15IE = 1;	// interrupcion para el S3
    CNEN2bits.CN16IE = 1;	// interrupcion para el S6
    CNEN2bits.CN23IE = 1;   // interrupcion para el S5
    CNEN2bits.CN19IE = 1;   // interrupcion para el S4
    CNEN2bits.CN20IE = 1;   // Interrupcion para el gatillo del joystick
    CNEN1bits.CN10IE = 1;   // Interrupcion para el boton derecho del joystick
    CNEN2bits.CN21IE = 1;   // Interrupcion para el boton central del joystick

	IEC1bits.CNIE = 1;      // habilitacion de la interrupcion general de CN
	IFS1bits.CNIF = 0;      // Puesta a 0 del flag IF del modulo
}

// Rutina de atencion a las interrupciones de CN (Placa y Joystick)
void _ISR_NO_PSV _CNInterrupt()
{
    // S3 Para el cronómetro y lo pone en marcha
    if (!PORTDbits.RD6) { 
        T7CONbits.TON = !T7CONbits.TON;
    }
    
    // S6 Resetea el cronómetro
    if (!PORTDbits.RD7) { 
        inic_crono();
        T7CONbits.TON = 0;
    }
    
    // S4 scroll hacia abajo
    if (!PORTDbits.RD13){ 
        flagScroll = 1;
    }
    
    // S5 scroll hacia arriba
    if (!PORTAbits.RA7){  
        flagScroll = 0;
    }
    
    // Boton central Joystick devuelve el brazo a su posicion de inicio
    if (!PORTDbits.RD15) {  
        flag_ini = 1;
        modo_control = 0;
    }
    
    // Gatillo del Joystick cierra la pinza
    if (!PORTDbits.RD14 && modo_control == 1) {  
        flag_gatillo = 1;
    } else {
        flag_gatillo = 0;
    }
    
    // Boton lateral del Joystick abre la pinza
    if (!PORTGbits.RG8 && modo_control == 1) {
        flag_derecho = 1;
    } else {
        flag_derecho = 0;
    }
    
	IFS1bits.CNIF = 0;		
}