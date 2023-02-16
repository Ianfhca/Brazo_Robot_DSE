#include "p24HJ256GP610A.h"
#include "commons.h"

void inic_Timer7 ()
{
    TMR7 = 0 ; 	// Inicializar el registro de cuenta
    PR7 =  50000;	// Periodo del timer MAX = 2^16 = 65.536
		// Queremos que cuente 10 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 10 ms se necesitan XXX ciclos.
    T7CONbits.TCKPS = 1;	// escala del prescaler 1:8
    T7CONbits.TCS = 0;	// reloj interno
    T7CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    T7CONbits.TON = 1;	// puesta en marcha del timer
    
    IEC3bits.T7IE = 1;
}



unsigned int mili, deci, seg, min;
void inic_crono()	
// inicializacion de las variables del cronometro
{
	//completad codigo
    mili = 0;
    deci = 0;
    seg = 0;
    min = 0;
}

void crono()
{
    if (mili == 100) {
        LATAbits.LATA2 = !LATAbits.LATA2;
        mili = 0;
        deci++;
    }
    if (deci == 10) {
        LATAbits.LATA0 = !LATAbits.LATA0;
        deci = 0;
        seg++;
    }
    if (seg == 60) {
        seg = 0;
        min++;
    }
}

void _ISR_NO_PSV _T7Interrupt()	
// control del tiempo espera 10 ms y luego actualiza
{
    mili += 10;  
    IFS3bits.T7IF = 0;
}

void delay_ms(int milisegundos) 
{
    TMR9 = 0 ; 	// Inicializar el registro de cuenta
    unsigned long ciclos = 40000 * milisegundos;
    unsigned long max_ciclos = 65536;
    float prescaler = (float)(ciclos/max_ciclos);
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
    
    T9CONbits.TON = 1;	// puesta en marcha del timer
    
    IFS3bits.T9IF = 0;
    
    while (!IFS3bits.T9IF) { // esperar hasta que cuente los milisegundos
        // se activa IFS3bits.T9IF = 1;
    }
    IFS3bits.T9IF = 0;
}

void delay_us(int microsegundos) 
{
    TMR9 = 0 ; 	// Inicializar el registro de cuenta
    unsigned long ciclos = 40 * microsegundos;
    unsigned long max_ciclos = 65536;
    float prescaler = (float)(ciclos/max_ciclos);
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
    
    T9CONbits.TON = 1;	// puesta en marcha del timer
    
    IFS3bits.T9IF = 0;
    
    while (!IFS3bits.T9IF) { // esperar hasta que cuente los milisegundos
        // se activa IFS3bits.T9IF = 1;
    }
    IFS3bits.T9IF = 0;
}
