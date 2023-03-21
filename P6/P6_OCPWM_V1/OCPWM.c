/*
Funciones relacionadas con el modulo OC para generar señal PWM

Comentarios!!!!!!!!!!!!!!

Autorxs:
Fecha:
*/

#include "p24HJ256GP610A.h"
#include "commons.h"

unsigned int DUTY_MIN=xx;	// valor minimo y maximo de DUTY. Se calculan 
unsigned int DUTY_MAX=xx;	// mediante los "define" PR20ms, MINPWM y MAXPWM
                              

void inic_OC1 ()
{
    OC1CON=0;
    //OC1CONbits.OCM=0b000;     // deshabilitar OC1 
    //OC1CONbits.OCTSEL=0;      // seleccionar T2 para el OC
    
    OC1R =  xx; 		// Inicializar pulso con duracion intermedia
    OC1RS = OC1R;               // inicializar registro secundario

    OC1CONbits.OCM=0b110;       // habilitar OC1 en modo PWM
}7
