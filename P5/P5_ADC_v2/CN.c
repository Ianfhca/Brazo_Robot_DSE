/*
 Funciones relacionadas con el modulo CN: inicializacion del modulo
 y rutina de atencion.
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "timers.h"

// Funcion para inicializar el modulo CN
//==================
void inic_CN()
{
  	CNEN1bits.CN15IE = 1;	// interrupcion para el S3
    CNEN2bits.CN16IE = 1;	// interrupcion para el S6

	IEC1bits.CNIE = 1;      // habilitacion de la interrupcion general de CN
	IFS1bits.CNIF = 0;      // Puesta a 0 del flag IF del modulo
}

// RUTINA DE ATENCION A LA INTERRUPCION DE CN
//==============================
void _ISR_NO_PSV _CNInterrupt()
{
    if (!PORTDbits.RD6) { // S3 Para el cronómetro y lo pone en marcha
        T7CONbits.TON = !T7CONbits.TON;
    }

    if (!PORTDbits.RD7) { // S6 Resetea el cronómetro
        inic_crono();
        T7CONbits.TON = 0;
    }

	IFS1bits.CNIF = 0;		
}