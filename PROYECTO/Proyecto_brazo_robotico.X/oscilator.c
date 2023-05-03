/*
 * Fichero: oscilator.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Inicializacion del modulo oscilator
 * 
 * Para ver la configuracion:
 * Barra herramientas: Windows -> PIC Memory Views -> Configuration Bits
 */

// Definiciones necesarias
#include "p24HJ256GP610A.h"

// Configuracion inicial
# pragma config FNOSC=PRIPLL   // Oscilador primario (XT, HS, EC) w/PLL
# pragma config FCKSM=CSECMD   // Habilitar el Clok Switching
# pragma config OSCIOFNC=OFF   // OSC2 es clock
# pragma config POSCMD=XT      // XT Oscilador
# pragma config FWDTEN=OFF     // Watchdog Timer: Deshabilitado

// Reloj a 80 MHz para que las instrucciones se ejecuten a 40 MHz.
// Fosc= Fin*M(N1*N2),  Fcy=Fosc/2
// Fosc= 8*40/(2*2)= 80MHz for 8MHz input clock
void inic_oscilator() {
	PLLFBD = 40-2;			    // M=PLLFBD+2
	CLKDIVbits.PLLPOST = 0;		// N2=(PLLPOST+1)*2
	CLKDIVbits.PLLPRE = 0;		// N1=PLLPRE+2
	RCONbits.SWDTEN = 0;		// Deshabilitar el temporizador Watch Dog

	// Clock switch para incorporar PLL
	__builtin_write_OSCCONH(0x03);	// Iniciar el Clock Switch en primario 
    // Oscilador con PLL (NOSC=0b011)
	__builtin_write_OSCCONL(0x01);	// Iniciar el Clock Switching

	while (OSCCONbits.COSC != 0b011);// Esperar a que ocurra el Clock Switching

	// Esperar a que el PLL se bloquee
	while(OSCCONbits.LOCK !=1) {}
}
