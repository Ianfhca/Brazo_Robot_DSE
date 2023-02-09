/*
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
 * Comentar que hace el programa (enunciado)!!!!!!!!!!!!!
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Egileak: Izaskun
Data: Febrero 2020
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "GPIO.h"
#include "CN.h"



int main()
{
 inic_leds();        
 inic_pulsadores();         
 
 LATAbits.LATA6 = 1; 

 while (PORTDbits.RD6) { //Espera al pulsador S3 por encuesta
     
 }
 
 LATAbits.LATA6 = 0;
 LATAbits.LATA5 = 1;
 inic_CN();  
 
 while(1)
	{
		
	}
}
