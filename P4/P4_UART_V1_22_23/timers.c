#include "p24HJ256GP610A.h"
#include "commons.h"
#include "LCD.h"
#include "memoria.h"
#include "utilidades.h"

void inic_Timer7 ()
{
    TMR7 = 0 ; 	// Inicializar el registro de cuenta
    PR7 =  50000;
	
    T7CONbits.TCKPS = 1;	// escala del prescaler 1:8
    T7CONbits.TCS = 0;	// reloj interno
    T7CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    T7CONbits.TON = 1;	// puesta en marcha del timer
    
    IEC3bits.T7IE = 1;
}

void inic_Timer5 ()
{
    TMR5 = 0; 	// Inicializar el registro de cuenta
    PR5 =  12500;	// Periodo del timer MAX = 2^16 = 65.536

    T5CONbits.TCKPS = 1;	// escala del prescaler 1:8
    T5CONbits.TCS = 0;	// reloj interno
    T5CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    T5CONbits.TON = 1;	// puesta en marcha del timer
    
    IEC1bits.T5IE = 1;
}

unsigned int mili, deci, seg, min;
int flag_inic_crono = 0;
void inic_crono()	
// inicializacion de las variables del cronometro
{
    mili = 0;
    deci = 0;
    seg = 0;
    min = 0;
    
    flag_inic_crono = 1;
    
}

void comprobar_inic_crono(){
    if (flag_inic_crono==1){
        conversion_tiempo(&Ventana_LCD[1][13], deci);
        conversion_tiempo(&Ventana_LCD[1][7], min);
        conversion_tiempo(&Ventana_LCD[1][10], seg);
            
        flag_inic_crono = 0;
    }
}

void crono()
{
    if (mili == 100) {
        LATAbits.LATA2 = !LATAbits.LATA2;
        mili = 0;
        deci++;
        
        conversion_tiempo(&Ventana_LCD[1][13], deci);
    }
    if (deci == 10) {
        LATAbits.LATA0 = !LATAbits.LATA0;
        deci = 0;
        seg++;
        
        conversion_tiempo(&Ventana_LCD[1][10], seg);
    }
    if (seg == 60) {
        seg = 0;
        min++;
        
        conversion_tiempo(&Ventana_LCD[1][7], min);
    }
}

void _ISR_NO_PSV _T7Interrupt()	
// control del tiempo espera 10 ms y luego actualiza
{
    mili += 10;  
    IFS3bits.T7IF = 0;
}


int estado = 0, i = 0;
// control del tiempo espera 2.5 ms y luego actualiza
void _ISR_NO_PSV _T5Interrupt()	
{
    // Maquina de Estados:
    switch(estado){
        case 0:
            i = 0;
            lcd_cmd(0x80);
            estado=1;
        break;
        
        case 1:
            lcd_data(Ventana_LCD[0][i]);
            i++;
            if (i == 16) 
            estado=2;
            break;
            
        case 2:
            i = 0;
            lcd_cmd(0xC0);
            estado = 3;
            break;
            
        case 3:
            lcd_data(Ventana_LCD[1][i]);
            i++;
            if (i == 16)  
            estado = 0;
            break;
                    
    }
    
    IFS1bits.T5IF = 0;
}

void Delay_ms(int milisegundos) 
{
    TMR9 = 0 ; 	// Inicializar el registro de cuenta
    unsigned long ciclos = (unsigned long)40000 * milisegundos;
    unsigned long max_ciclos = 65536;
    unsigned long prescaler = ciclos/max_ciclos;
    if (prescaler < 1) {
        PR9 =  ciclos;	// Periodo del timer 40000 * milisegundos
        T9CONbits.TCKPS = 0;	// escala del prescaler 1:1
    } else if (prescaler < 8) {
        PR9 =  ciclos/8;	// Periodo del timer 40000 * milisegundos / 8
        T9CONbits.TCKPS = 1;	// escala del prescaler 1:8
    } else if (prescaler < 64) {
        PR9 =  ciclos/64;	// Periodo del timer 40000 * milisegundos / 64
        T9CONbits.TCKPS = 2;	// escala del prescaler 1:64
    } else if (prescaler < 256) {
        PR9 =  ciclos/256;	// Periodo del timer 40000 * milisegundos / 256
        T9CONbits.TCKPS = 3;	// escala del prescaler 1:256
    }
    T9CONbits.TCS = 0;	// reloj interno
    T9CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IFS3bits.T9IF = 0;
    
    T9CONbits.TON = 1;	// puesta en marcha del timer    
    
    while (!IFS3bits.T9IF) { // esperar hasta que cuente los milisegundos
        // se activa IFS3bits.T9IF = 1;
    }
    IFS3bits.T9IF = 0;
    T9CONbits.TON = 0;
}

void Delay_us(int microsegundos) 
{
    TMR9 = 0 ; 	// Inicializar el registro de cuenta
    unsigned long ciclos = (unsigned long)40 * microsegundos;
    unsigned long max_ciclos = 65536;
    unsigned long prescaler = ciclos/max_ciclos;
    if (prescaler < 1) {
        PR9 =  ciclos;	// Periodo del timer 40000 * milisegundos
        T9CONbits.TCKPS = 0;	// escala del prescaler 1:1
    } else if (prescaler < 8) {
        PR9 =  ciclos/8;	// Periodo del timer 40000 * milisegundos / 8
        T9CONbits.TCKPS = 1;	// escala del prescaler 1:8
    } else if (prescaler < 64) {
        PR9 =  ciclos/64;	// Periodo del timer 40000 * milisegundos / 64
        T9CONbits.TCKPS = 2;	// escala del prescaler 1:64
    } else if (prescaler < 256) {
        PR9 =  ciclos/256;	// Periodo del timer 40000 * milisegundos / 256
        T9CONbits.TCKPS = 3;	// escala del prescaler 1:256
    }
    T9CONbits.TCS = 0;	// reloj interno
    T9CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IFS3bits.T9IF = 0;
    
    T9CONbits.TON = 1;	// puesta en marcha del timer
    
    while (!IFS3bits.T9IF) { // esperar hasta que cuente los milisegundos
        // se activa IFS3bits.T9IF = 1;
    }
    IFS3bits.T9IF = 0;
    T9CONbits.TON = 0;
}
