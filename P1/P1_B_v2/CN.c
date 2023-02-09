/*
 Funciones relacionadas con el modulo CN: inicializacion del modulo
 y rutina de atencion.

Egileak: Izaskun
Data: Febrero 2020
*/

#include "p24HJ256GP610A.h"
#include "commons.h"

// Funcion para inicializar el modulo CN
//==================
void inic_CN()
{
  	CNEN2bits.CN16IE = 1;	// habilitacion de la interrupcion del pin CN16
                            // que es al que esta conectado el pulsador S6
    CNEN2bits.CN19IE = 1;   // habilitacion de la interrupcion del pin CN19
                            // que es al que esta conectado el pulsador S4
    CNEN2bits.CN23IE = 1;   // habilitacion de la interrupcion del pin CN23
                            // que es al que esta conectado el pulsador S5
    
	IEC1bits.CNIE = 1;      // habilitacion de la interrupcion general de CN
	IFS1bits.CNIF = 0;      // Puesta a 0 del flag IF del modulo
}

int pulseS4 = 0;
int pulseS5 = 0;
int pulseS6 = 0;


// RUTINA DE ATENCION A LA INTERRUPCION DE CN
//==============================
void _ISR_NO_PSV _CNInterrupt()
{
		if (!PORTDbits.RD13) //S4  
        {
            LATAbits.LATA0 = !LATAbits.LATA0; //cambia estado de D3
            pulseS4++;
        }
        if (!PORTAbits.RA7)  //S5
        {
            LATAbits.LATA2 = !LATAbits.LATA2; //cambia estado de D5
            pulseS5++;
        }
        if (!PORTDbits.RD7)  //S6
        {
            LATAbits.LATA4 = !LATAbits.LATA4; //cambia estado de D7
            pulseS6++;
        }
	IFS1bits.CNIF = 0;		
}

