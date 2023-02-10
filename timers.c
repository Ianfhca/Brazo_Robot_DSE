#include "p24HJ256GP610A.h"
#include "commons.h"

void inic_Timer7 ()
{
    TMR7 = 0 ; 	// Inicializar el registro de cuenta
    PR7 =  0xc350;	// Periodo del timer 50000
		// Queremos que cuente 10 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 10 ms se necesitan XXX ciclos.
    T7CONbits.TCKPS = 1;	// escala del prescaler 1:8
    T7CONbits.TCS = 0;	// reloj interno
    T7CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    T7CONbits.TON = 1;	// puesta en marcha del timer
}	

//void init_Timer9(unsigned int )

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

int max = 2**16;

void calculate_prescaler_mst(unsigned int mst, unsigned int tipe) {
    long wait = mst*(40000/(1000**tipe));
    
    if (wait <= max){
        T9CONbits.TCKPS1 = 0;
        T9CONbits.TCKPS0 = 0;	
        wait = wait;
    }else if (wait <= max*8) {
        T9CONbits.TCKPS1 = 0;	
        T9CONbits.TCKPS0 = 1;
        wait = wait/8;
    }else if (wait <= max*64) {
        T9CONbits.TCKPS1 = 1;	
        T9CONbits.TCKPS0 = 0;	
        wait = wait/64;
    }else {
        T9CONbits.TCKPS1 = 1;	
        T9CONbits.TCKPS0 = 1;
        wait = wait/256;
    }
     PR7 =  wait;
    
}

void delay_10ms()	// detecta que el timer ha llegado a 10 milisegundos
{
	//completad codigo
    while (!IFS3bits.T7IF);	// encuesta continua    
    // ACORDARSE )DE PONER EL IF A 0 
    mili += 10;
    IFS3bits.T7IF = 0;
}

int maximun_delay = 100;
void Delay_ms(unsigned int mst) {
    //Desechamos si mst no esta dentro del rangos
    if (mst > maximun_delay) return;
    Nop(); //Ponemos puntos de ruptura
    //Calculamos el valor del prescaler y el TRP
    calculate_prescaler_mst(mst, 0);
    //Habilitamos interrupciones en el temporizador T9
    IEC3bits.T9IE = 1;
    //Pòner en marcha el timer
    T9CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    T9CONbits.TON = 1;	// puesta en marcha del timer
    //Inicializamos el temporizador T9 a 0
    TMR9 = 0;
    //Esperamos por encuesta al T9
    while (!IFS3bits.T9IF);
    //Bajamos el flag if del T9 
    IFS3bits.T9IF = 0;
    //Deshabilitamos las interrupciones en T9
    IEC3bits.T9IE = 0;
    Nop(); //Ponemos puntos de ruptura
}

int maximun_delay_us = 100;
void Delay_us(unsigned int ust) {
    //Desechamos si mst no esta dentro del rangos
    if (ust > maximun_delay_us) return;
    Nop(); //Ponemos puntos de ruptura
    //Calculamos el valor del prescaler y el TRP
    calculate_prescaler_mst(ust, 1);
    //Habilitamos interrupciones en el temporizador T9
    IEC3bits.T9IE = 1;
    //Pòner en marcha el timer
    T9CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    T9CONbits.TON = 1;	// puesta en marcha del timer
    //Inicializamos el temporizador T9 a 0
    TMR9 = 0;
    //Esperamos por encuesta al T9
    while (!IFS3bits.T9IF);
    //Bajamos el flag if del T9 
    IFS3bits.T9IF = 0;
    //Deshabilitamos las interrupciones en T9
    IEC3bits.T9IE = 0;
    Nop(); //Ponemos puntos de ruptura
}

void cronometro()	
// control del tiempo espera 10 ms y luego actualiza
{
    delay_10ms();	// espera a que pasen 10 milisegundos
  // completad codigo
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

