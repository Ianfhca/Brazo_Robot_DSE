/*
 * File:   ADC1.c
 * Author: Ian
 *
 * Created on 28 de abril de 2023, 15:07
 */
#include <p24HJ256GP610A.h>
#include "LCD.h"
#include "memoria.h"
#include "utilidades.h"
#include "ADC1.h"
#include "commons.h"
#include "PWM.h"

void inic_ADC1 (void) {
// Inicializacion registro control AD1CON1
AD1CON1 = 0;       // todos los campos a 0

// Salida de 12 bits o 10 bits
//AD1CON1bits.AD12B = 0;  

// Comienzo digitalizacion automatico
// 111=Auto-convert / 010=TMR3 ADC1 y TMR5 ADC2 / 001=INT0 / 000= SAMP 
AD1CON1bits.SSRC = 2; // Valor 2 para sincronizar con el timer 3 		

// Muestreo simultaneo o secuencial
//AD1CON1bits.SIMSAM = 0; 
             
// Comienzo muestreo automático o por programa (SAMP=1) 		
AD1CON1bits.ASAM = 1;

                    
// Inicializacion registro control AD1CON2
AD1CON2 = 0;  // todos los campos a 0


// Inicializacion registro control AD1CON3
AD1CON3 = 0;    // todos los campos a 0
// Reloj con el que funciona el ADC:  0= reloj CPU; 1= RC erlojua 
//AD1CON3bits.ADRC = 0;  // 
AD1CON3bits.SAMC = 31;   // Tiempo muestreo = numero de Tad 
AD1CON3bits.ADCS = 3;   // Relacion entre TAD y Tcy TAD = Tcy(ADCS+1)


// Inicializacion registro control AD1CON4
AD1CON4 = 0;


// Inicializacion registro AD1CHS123
AD1CHS123 = 0;	//seleccion del canal 1,2 eta 3


// Inicializacion registro AD1CHS0
AD1CHS0 = 0;
AD1CHS0bits.CH0SA = 5; // Elige la entrada analogica conectada, 5: potenciometro

//AD1CHS0bits.CH0SB = 0;


// Inicializacion registros AD1CSS 
// Si escaneo secuencial 1, si no 0

AD1CSSH = 0;   // 16-31 
AD1CSSL = 0;   // 0-15 

// Inicializacion registros AD1PCFG. Inicialmente todas AN como digitales
AD1PCFGH = 0xFFFF;      // 1= digital / 0= Analog
AD1PCFGL = 0xFFFF;      // Puerto B, todos digitales
// Inicializar como analogicas solo las que vayamos a usar
AD1PCFGLbits.PCFG5 = 0;   // potenciometro anal?gico = 0
AD1PCFGLbits.PCFG4 = 0;   // sensor temperatura anal?gico = 0
AD1PCFGLbits.PCFG0 = 0; //Px
AD1PCFGLbits.PCFG1 = 0; //Py
AD1PCFGLbits.PCFG2 = 0; //Palanca

// Bits y campos relacionados con las interrupciones
IFS0bits.AD1IF = 0;    
IEC0bits.AD1IE = 1;    
//IPC3bits.AD1IP=xx; // Registro para controlar la prioridad    

//AD1CON
AD1CON1bits.ADON = 1;  // Habilitar el modulo ADC
}

void _ISR_NO_PSV _ADC1Interrupt() {
// Interrumpe el ADC y se recoge el valor
    recoger_valorADC1();
    IFS0bits.AD1IF = 0;
}

unsigned long num_interrupt = 0;
// Funcion que recoge el valor del adc cada milisegundo, se llama en la rutina de atencion de T3
void recoger_valorADC1 () {   
     static unsigned int numMuestras = 0;
    
    switch(AD1CHS0bits.CH0SA) {   
        case 0:
            tabla_Px[numMuestras] = ADC1BUF0;
            AD1CHS0bits.CH0SA = 1; // elige el Py
            break;
        
        case 1:
            tabla_Py[numMuestras] = ADC1BUF0;
            AD1CHS0bits.CH0SA = 2; // elige el Py
            break;    
        case 2:
            tabla_Palanca[numMuestras] = ADC1BUF0;
            AD1CHS0bits.CH0SA = 4; // elige el Py
            break;
        
            case 4:
                tabla_Temp[numMuestras] = ADC1BUF0;
                AD1CHS0bits.CH0SA = 5; // elige el potenciometro
                break;
            
            case 5:
                tabla_Pot[numMuestras] = ADC1BUF0;
                numMuestras++;
                AD1CHS0bits.CH0SA = 0; // elige el sensor de temperatura
                break;
        }
         
         if (numMuestras==8){
             numMuestras = 0;
             flag_muestras = 1;
             AD1CON1bits.ADON = 0;  //Deshabilita ADC
         }
    
    num_interrupt++;
}

//Variables de arrays para almacenar las muestras y flags
int flag_ADC1 = 0;
int flag_muestras = 0;
unsigned int tabla_Pot[8];
unsigned int tabla_Temp[8];
unsigned int tabla_Px[8];
unsigned int tabla_Py[8];
unsigned int tabla_Palanca[8];

unsigned int mediaMuestrasPot = 0, mediaMuestrasTemp = 0, mediaMuestrasPx = 0,
            mediaMuestrasPy = 0, mediaMuestrasPalanca = 0;

void calcular_media_muestras(){
   
    mediaMuestrasPot = 0;
    mediaMuestrasTemp = 0; 
    mediaMuestrasPx = 0;
    mediaMuestrasPy = 0; 
    mediaMuestrasPalanca = 0;
    
    
    unsigned int i;

    for(i=0; i<8; i++){
        mediaMuestrasPot += tabla_Pot[i];
        mediaMuestrasTemp += tabla_Temp[i];
        mediaMuestrasPx += tabla_Px[i];
        mediaMuestrasPy += tabla_Py[i];
        mediaMuestrasPalanca += tabla_Palanca[i];
    }

    mediaMuestrasPot = mediaMuestrasPot/8;
    mediaMuestrasTemp = mediaMuestrasTemp/8;
    mediaMuestrasPx = mediaMuestrasPx/8;
    mediaMuestrasPy = mediaMuestrasPy/8;
    mediaMuestrasPalanca = mediaMuestrasPalanca/8;
    
    conversion_4digitos(&pantalla[4][7], mediaMuestrasPot);
    conversion_4digitos(&pantalla[5][7], mediaMuestrasTemp);
    conversion_4digitos(&pantalla[7][4], mediaMuestrasPx);
    conversion_4digitos(&pantalla[8][4], mediaMuestrasPy);
    conversion_4digitos(&pantalla[9][4], mediaMuestrasPalanca);
    //conversion_adc(&Ventana_LCD[0][12],mediaMuestrasTemp);
    
        

    AD1CON1bits.ADON = 1; //Vuelve a habilitar ADC y comienza nuevo muestreo
}

void controlarServos(){
    DUTY[0] = relacion_adc_pwm(mediaMuestrasPx);
    DUTY[1] = relacion_adc_pwm(mediaMuestrasPy);
    DUTY[2] = relacion_adc_pwm(mediaMuestrasPalanca);
    DUTY[3] = relacion_adc_pwm(mediaMuestrasPot);
    
    flag_servo = 1;
}