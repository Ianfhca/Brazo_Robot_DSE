/*
Cronometro mediante leds.
Control de temporizador por encuesta.
Se utilizan los siguientes leds:
-------- // COMPLETAD!!!!!!

Autorxs:
Fecha: 
*/

#include "p24HJ256GP610A.h"

#include "oscilator.h"
#include "GPIO.h"
#include "timers.h"
#include "commons.h"

// TCKPS para el prescaler

int main()
{
    
	inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz
	
	inic_leds();	// Inicializacio³n leds: sentido y valor inicial.
    
    inic_pulsadores(); //Inicializacion de pulsadores
	
    while(PORTDbits.RD7){ //Esperar hasta pulsar S6
        
    }
    
	inic_crono();	//Inicializacion variables cronometro.

	inic_Timer7();	// Inicializacion T7 con un periodo de 10 milisegundos.
    
    inic_CN();
    
    int stop = 0;
    LATAbits.LATA5 = 0;
    
	while(1) {
        
	}
	return (0);
}
	





