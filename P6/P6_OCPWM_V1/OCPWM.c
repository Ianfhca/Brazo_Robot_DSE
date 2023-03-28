/*
Funciones relacionadas con el modulo OC para generar señal PWM

Comentarios!!!!!!!!!!!!!!

Autorxs:
Fecha:
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "utilidades.h"
#include "ADC1.h"

unsigned int DUTY_MIN = (PR20ms/20) * MINPWM;	// Valor minimo y maximo de DUTY. Se calculan 
unsigned int DUTY_MAX = (PR20ms/20) * MAXPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM
unsigned int flag_servo = 0;    
unsigned int modo_control = 0; //Controla el modo de control del pwm: 0-UART, 1-ADC

void inic_OC1 ()
{
    OC1CON=0;
    OC1CONbits.OCM=0b000;       // Deshabilitar OC1 
    //OC1CONbits.OCTSEL=0;        // Seleccionar T2 para el OC
    
    OC1R = (DUTY_MAX+DUTY_MIN)/2; // Inicializar pulso con duracion intermedia
    OC1RS = OC1R;                 // Inicializar registro secundario

    //OC1RS corresponde al ciclo actual
    //OC1R es el del siguiente ciclo
    OC1CONbits.OCM=0b110;         // Habilitar OC1 en modo PWM valor 6
}

void mostrar_OC1() {
    conversion_4digitos(&Ventana_LCD[0][12], OC1RS);
}

void relacion_adc_pwm(unsigned int valor_adc) {
    OC1RS = valor_adc*((float)(DUTY_MAX - DUTY_MIN)/1023) + DUTY_MIN;
    
}
