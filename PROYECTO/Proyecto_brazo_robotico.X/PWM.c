/*
Funciones relacionadas con el modulo OC para generar se�al PWM

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

unsigned int DUTY_MIN[5] = {(PR20ms/20) * MINPWM, (PR20ms/20) * MINPWM, 
                            (PR20ms/20) * MINPWM, (PR20ms/20) * MINPWM, 
                            712};	// Valor minimo y maximo 
                            // de DUTY. Se calculan 
unsigned int DUTY_MAX[5] = {(PR20ms/20) * MAXPWM, (PR20ms/20) * MAXPWM,
                            (PR20ms/20) * MAXPWM, (PR20ms/20) * MAXPWM,
                            1412};	// Mediante los "define"
                            // PR20ms, MINPWM y MAXPWM
unsigned int flag_servo = 0; // flag que indica cuando hay que actualizar la visualizacion de los dutys
unsigned int flag_objetivo = 0; // flag que indica que hay que acercar el valor actual del duty al valor objetivo
unsigned int modo_control = 0; //Controla el modo de control del pwm: 0-UART, 1-ADC
unsigned int DUTY[5] = {D1, D2, D3, D4, D5};; // Valor medio DUTY = 812
unsigned int duty_palanca = 1208;
unsigned int servoActual = 0; // Variable que controla que servomotor se esta controlando

unsigned int movActual = 0;
unsigned int secuencia[5][5];

void mostrar_duty() {
    int i;
    for (i = 0; i < 5; i++)
        conversion_4digitos(&pantalla[i+11][9], DUTY[i]);
}

int relacion_adc_pwm(unsigned int valor_adc, int servo) {
    return (valor_adc*((float)(DUTY_MAX[servo] - DUTY_MIN[servo])/1023) + DUTY_MIN[servo]);    
}