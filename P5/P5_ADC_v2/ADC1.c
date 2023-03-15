// Funciones para el modulo ADC1
#include <p24HJ256GP610A.h>
#include "LCD.h"
#include "memoria.h"
#include "utilidades.h"
#include "ADC1.h"
#include "commons.h"

void inic_ADC1 (void)
{
// Inicializacion registro control AD1CON1
AD1CON1 = 0;       // todos los campos a 0

// Salida de 12 bits o 10 bits
//AD1CON1bits.AD12B = 0;  

// Comienzo digitalizacion automatico
// 111=Auto-convert / 010=TMR3 ADC1 y TMR5 ADC2 / 001=INT0 / 000= SAMP 
AD1CON1bits.SSRC = 7;    		

// Muestreo simultaneo o secuencial
//AD1CON1bits.SIMSAM = 0; 
             
// Comienzo muestreo automÃ¡tico o por programa (SAMP=1) 		
//AD1CON1bits.ASAM = 0;

                    
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
AD1CHS0bits.CH0SA = 5; // elige la entrada analogica conectada

//AD1CHS0bits.CH0SB = 0;


// Inicializacion registros AD1CSS 
// Si escaneo secuencial 1, si no 0

AD1CSSH = 0;   // 16-31 
AD1CSSL = 0;   // 0-15 

// Inicializacion registros AD1PCFG. Inicialmente todas AN como digitales
AD1PCFGH = 0xFFFF;      // 1= digital / 0= Analog
AD1PCFGL = 0xFFFF;      // Puerto B, todos digitales
// Inicializar como analogicas solo las que vayamos a usar
AD1PCFGLbits.PCFG5 = 0;   // potenciometro analógico = 0
AD1PCFGLbits.PCFG4 = 0;   // sensor temperatura analógico = 0

// Bits y campos relacionados con las interrupciones
IFS0bits.AD1IF = 0;    
IEC0bits.AD1IE = 1;    
//IPC3bits.AD1IP=xx; // Registro para controlar la prioridad    

//AD1CON
AD1CON1bits.ADON = 1;  // Habilitar el modulo ADC
}


// comienzo del muestreo por programa
void comienzo_muestreo ()
{
    AD1CON1bits.SAMP = 1;
}

// Funcion que recoge el valor del convertidor por encuesta
void recoger_valorADC1 ()
{   
    static unsigned int valor_pot; // Variable que almacena el valor del potenciomentro (en interrupcion sera global)
    static unsigned int valor_temp; // Variable que almacena el valor de la temperatura (en interrupcion sera global)
    if (AD1CON1bits.DONE){
         
        switch(AD1CHS0bits.CH0SA) {
            case 4:
                valor_temp = ADC1BUF0;
                conversion_adc(&Ventana_LCD[0][12],valor_temp);
                AD1CHS0bits.CH0SA = 5; // elige el potenciometro
                break;
            
            case 5:
                valor_pot = ADC1BUF0;
                conversion_adc(&Ventana_LCD[0][3],valor_pot);
                AD1CHS0bits.CH0SA = 4; // elige el sensor de temperatura
                break;
        }
         
         comienzo_muestreo();
    }
}

int flag_ADC1 = 1;
static unsigned int valor_pot; // Variable que almacena el valor del potenciomentro (en interrupcion sera global)
static unsigned int valor_temp; // Variable que almacena el valor de la temperatura
int flag_muestras;
unsigned int tabla_pot[8];
unsigned int tabla_temp[8];

void recoger_valorADC1_int() {
    if (AD1CON1bits.DONE){
    switch(AD1CHS0bits.CH0SA) {
            case 4:
                valor_temp = ADC1BUF0;
                
                conversion_adc(&Ventana_LCD[0][12],valor_temp);
                AD1CHS0bits.CH0SA = 5; // elige el potenciometro
                break;
            
            case 5:
                valor_pot = ADC1BUF0;
                conversion_adc(&Ventana_LCD[0][3],valor_pot);
                AD1CHS0bits.CH0SA = 4; // elige el sensor de temperatura
                break;
        }
    
         comienzo_muestreo();
    }
}

// Rutina de atención del ADC1
void _ISR_NO_PSV _ADC1Interrupt() {
    if(flag_ADC1 == 1) {
        recoger_valorADC1_int();
    } 
}

