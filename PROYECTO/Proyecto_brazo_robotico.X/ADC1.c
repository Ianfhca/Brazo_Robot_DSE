/*
 * Fichero: ADC1.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Funciones relacionadas con el convertidor de senales
 * analogicas a digitales (ADC)
 * 
 */

// Definiciones necesarias
#include <p24HJ256GP610A.h>
#include "LCD.h"
#include "memoria.h"
#include "utilidades.h"
#include "ADC1.h"
#include "commons.h"
#include "PWM.h"

// Inicializa los registros necesarios para el correcto funcionamiento del ADC1
void inic_ADC1 (void) {
// Inicializacion registro control AD1CON1
AD1CON1 = 0;    // Todos los campos a 0

// Comienzo digitalizacion automatico
// 111 = Auto-convert / 010 = TMR3 ADC1 y TMR5 ADC2 / 001 = INT0 / 000 = SAMP 
AD1CON1bits.SSRC = 2; // Valor 2 para sincronizar con el timer 3 		

// Comienzo muestreo automático o por programa (SAMP=1) 		
AD1CON1bits.ASAM = 1;
                
// Inicializacion registro control AD1CON2
AD1CON2 = 0;    // Todos los campos a 0

// Inicializacion registro control AD1CON3
AD1CON3 = 0;    // Todos los campos a 0

// Reloj con el que funciona el ADC:  0 = reloj CPU; 1= RC reloj 
AD1CON3bits.SAMC = 31;  // Tiempo muestreo = Numero de Tad 
AD1CON3bits.ADCS = 3;   // Relacion entre TAD y Tcy TAD = Tcy(ADCS+1)

// Inicializacion registro control AD1CON4
AD1CON4 = 0;

// Inicializacion registro AD1CHS123
AD1CHS123 = 0;  // Seleccion del canal 1,2 eta 3

// Inicializacion registro AD1CHS0
AD1CHS0 = 0;
AD1CHS0bits.CH0SA = 5; // Elige la entrada analogica conectada, 5: potenciometro

// Inicializacion registros AD1CSS 
// Si escaneo secuencial 1, si no 0
AD1CSSH = 0;    // 16-31 
AD1CSSL = 0;    // 0-15 

// Inicializacion registros AD1PCFG. Inicialmente todas AN como digitales
AD1PCFGH = 0xFFFF;      // 1= digital / 0= Analog
AD1PCFGL = 0xFFFF;      // Puerto B, todos digitales

// Inicializar como analogicas solo las que vayamos a usar
AD1PCFGLbits.PCFG5 = 0; // Potenciometro analogico = 0
AD1PCFGLbits.PCFG4 = 0; // Sensor temperatura analogico = 0
AD1PCFGLbits.PCFG0 = 0; // Joystick Px analogico = 0
AD1PCFGLbits.PCFG1 = 0; // Joystick Py analogico = 0
AD1PCFGLbits.PCFG2 = 0; // Joystick Palanca analogico = 0

// Bits y campos relacionados con las interrupciones
IFS0bits.AD1IF = 0;    
IEC0bits.AD1IE = 1;  

// AD1CON
AD1CON1bits.ADON = 1;  // Habilitar el modulo ADC
}

// Rutina de atencion al ADC1
void _ISR_NO_PSV _ADC1Interrupt() {
    // El adc interrumpe junto con el timer 2 y se recoge el valor
    recoger_valor_ADC1();
    IFS0bits.AD1IF = 0;
}

// Funcion que recoge el valor del adc cada milisegundo, se llama en la 
// rutina de atencion de T3
unsigned long num_interrupt = 0;

void recoger_valor_ADC1 () {   
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
         
         if (numMuestras==8) {
             numMuestras = 0;
             flag_muestras = 1;
             AD1CON1bits.ADON = 0;  //Deshabilita ADC
         }
    
    num_interrupt++;
}

// Variables de arrays para almacenar las muestras y flags
int flag_ADC1 = 0;
int flag_muestras = 0;
unsigned int tabla_Pot[8];
unsigned int tabla_Temp[8];
unsigned int tabla_Px[8];
unsigned int tabla_Py[8];
unsigned int tabla_Palanca[8];

// Medias de cada ADC
unsigned int mediaMuestrasPot = 0, mediaMuestrasTemp = 0, mediaMuestrasPx = 0,
            mediaMuestrasPy = 0, mediaMuestrasPalanca = 0;

// Funcion para calcular la media de todas las muestras recogidas por los ADC
void calcular_media_muestras(){
   
    mediaMuestrasPot = 0;
    mediaMuestrasTemp = 0; 
    mediaMuestrasPx = 0;
    mediaMuestrasPy = 0; 
    mediaMuestrasPalanca = 0;
    
    
    unsigned int i;

    for (i=0; i<8; i++){
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
    
    AD1CON1bits.ADON = 1; //Vuelve a habilitar ADC y comienza nuevo muestreo
}

// Funcion que controla los distintos servo motores del brazo 
void controlar_servos(){
    static int  minx = MULT, miny = MULT, maxx =0, maxy = 0;
    
    // Control de Px
    if ((mediaMuestrasPx >= 490-DESV && mediaMuestrasPx <= 490+DESV)) {
        // Rango de no movimiento
    } else if (mediaMuestrasPx < 490-DESV && DUTY[0] >= DUTY_MIN[0]+VEL){
        minx--;
    } else if (mediaMuestrasPx > 490+DESV && DUTY[0] <= DUTY_MAX[0]-VEL){
        maxx++;
    } 
    
    // Cada vez que se alcanza maxx o minx se mueve el servo a velocidad VEL
    if (maxx >= MULT){
        maxx = 0;
        DUTY[0] += VEL;
    }
    if (minx <= 0){
        minx = MULT;
        DUTY[0] -= VEL;
    }
    
    // Control de Py
    if (mediaMuestrasPy >= 470-DESV && mediaMuestrasPy <= 470+DESV){
        // Rango de no movimiento
    } else if (mediaMuestrasPy < 470-DESV && DUTY[1] >= DUTY_MIN[1]+VEL){
        miny--;
    } else if (mediaMuestrasPy > 470+DESV && DUTY[1] <= DUTY_MAX[1]-VEL){
        maxy++;
    }
    // Cada vez que se alcanza maxy o miny se mueve el servo a velocidad VEL
    if (maxy >= MULT){
        maxy = 0;
        DUTY[1] += VEL;
    }
    if (miny <= 0){
        miny = MULT;
        DUTY[1] -= VEL;
    }
    
    duty_palanca = relacion_adc_pwm(mediaMuestrasPalanca, 2);
    DUTY[3] = relacion_adc_pwm(mediaMuestrasPot, 3);
}