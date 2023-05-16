/*
 * Fichero: PWM.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Funciones relacionadas con el modulo OC para generar señal PWM
 * 
 */

// Definiciones necesarias
#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "ADC1.h"
#include "PWM.h"
#include "utilidades.h"

// Valor minimo y maximo de DUTY
unsigned int DUTY_MIN[5] = {(PR20ms/20) * MINPWM, (PR20ms/20) * MINPWM, 
                            (PR20ms/20) * MINPWM, (PR20ms/20) * MINPWM, 
                            (PR20ms/20) * MINPWM};	
unsigned int DUTY_MAX[5] = {(PR20ms/20) * MAXPWM, (PR20ms/20) * MAXPWM,
                            (PR20ms/20) * MAXPWM, (PR20ms/20) * MAXPWM,
                            (PR20ms/20) * MAXPWM};
unsigned int flag_servo = 0;    // Para visualizar los DUTY
unsigned int flag_objetivo = 0; // Para acercar el valor del DUTY al objetivo
unsigned int modo_control = 0;  // Modo de control del pwm: 0-UART, 1-ADC
unsigned int DUTY[5] = {D1, D2, D3, D4, D5}; // Valor medio DUTY = 812
unsigned int duty_palanca = 1208;
unsigned int servoActual = 0; // Controla que servomotor se esta controlando

unsigned int movActual = 0;
unsigned int nMov = 0;
unsigned int secuencia[NMAXMOV][5];

// Actualiza el DUTY en la memoria del LCD
void mostrar_duty() {
    int i;
    for (i = 0; i < 5; i++)
        conversion_4digitos(&pantalla[i+11][9], DUTY[i]);
}

// Calcula el valor del servo en funcion al valor del ADC
int relacion_adc_pwm(unsigned int valor_adc, int servo) {
    return (valor_adc*((float)(DUTY_MAX[servo] - DUTY_MIN[servo])/1023) + DUTY_MIN[servo]);    
}