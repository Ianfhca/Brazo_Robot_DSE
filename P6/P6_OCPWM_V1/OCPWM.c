/*
Funciones relacionadas con el modulo OC para generar señal PWM

Comentarios!!!!!!!!!!!!!!

Autorxs:
Fecha:
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "commons_P6.h"

unsigned int DUTY_MIN = (PR20ms/20) * MINPWM;	// Valor minimo y maximo de DUTY. Se calculan 
unsigned int DUTY_MAX = (PR20ms/20) * MAXPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM
                              

void inic_OC1 ()
{
    OC1CON=0;
    //OC1CONbits.OCM=0b000;       // Deshabilitar OC1 
    //OC1CONbits.OCTSEL=0;        // Seleccionar T2 para el OC
    
    OC1R = (DUTY_MIN+DUTY_MAX)/2; // Inicializar pulso con duracion intermedia
    OC1RS = OC1R;                 // Inicializar registro secundario

    OC1CONbits.OCM=0b110;         // Habilitar OC1 en modo PWM valor 6
}
