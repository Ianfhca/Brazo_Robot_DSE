/*
Funciones relacionadas con el modulo OC para generar señal PWM

Comentarios!!!!!!!!!!!!!!

Autorxs:
Fecha:
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "ADC1.h"
#include "PWM.h"
#include "utilidades.h"

unsigned int DUTY_MIN = (PR20ms/20) * MINPWM;	// Valor minimo y maximo de DUTY. Se calculan 
unsigned int DUTY_MAX = (PR20ms/20) * MAXPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM
unsigned int flag_servo = 0; // flag que indica cuando hay que actualizar la visualizacion de los dutys
unsigned int modo_control = 0; //Controla el modo de control del pwm: 0-UART, 1-ADC
//unsigned int DUTY_MIT = (DUTY_MAX + DUTY_MIN)/2; // DUTY_MIT = 812
unsigned int DUTY[5] = {862, 1191, 1308, 445, 630}; // Valor medio DUTY = 812
unsigned int servoActual = 0; // Variable que controla que servomotor se esta controlando

void mostrar_duty() {
    int i;
    for (i = 0; i < 5; i++)
        conversion_4digitos(&pantalla[i+11][9], DUTY[i]);
}

int relacion_adc_pwm(unsigned int valor_adc) {
    return (valor_adc*((float)(DUTY_MAX - DUTY_MIN)/1023) + DUTY_MIN);    
}