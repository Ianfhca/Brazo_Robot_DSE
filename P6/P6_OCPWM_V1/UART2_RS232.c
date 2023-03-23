/* Funciones para trabajar con el modulo UART2
================================================
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "commons_P4.h"
#include "UART2_RS232.h"
#include "timers.h"
#include "memoria.h"
#include "OCPWM.h"



void inic_UART2 ()
{
    // Velocidad de transmision
    // Hay que hacer solo una de las dos asignaciones siguientes
	U2BRG = BAUD_RATEREG_2_BRGH1;
	// U2BRG = BAUD_RATEREG_2_BRGH0;

    // U2MODE: habilitar el modulo (UARTEN), 8 bits, paridad par (PDSEL),
    // 1 bit de stop (STSEL), BRGH ...
	U2MODE = 0;
	U2MODEbits.BRGH = 1;

    // U2STA: modo de interrupcion en el envio (UTXISEL), habilitacion del
    // envio (UTXEN), modo de interrupcion en la recepcion (URXISEL)
	U2STA = 0;

     // inicializacion de los bits IE e IF relacionados (IP si se quiere modificar)
	IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
	IEC1bits.U2RXIE = 1;
    IEC1bits.U2TXIE = 1;

    //IPC7bits.U2RXIP=xx;
	//IPC7bits.U2TXIP=xx;

     // interrupciones debidas a errores + bug
	IEC4bits.U2EIE = 0;
	U2STAbits.OERR = 0;

     // Habilitar el modulo y la linea TX.
     // Siempre al final y en ese orden!!!

	U2MODEbits.UARTEN = 1;    // habilitar UART2
	U2STAbits.UTXEN = 1;      // habilitar transmision tras habilitar modulo

	Delay_us(T_1BIT_US); 	// Esperar tiempo de 1 bit 
}

void _ISR_NO_PSV _U2RXInterrupt() {
    char c = U2RXREG;
    if (c == 'p' || c == 'P') {
        T7CONbits.TON = 0;
    }
    else if(c == 'i' || c=='I'){
        inic_crono();
    }
    else if(c=='c' || c=='C'){
        T7CONbits.TON = 1;
    }
    else if(c=='m' || c=='M'){
        if(OC1RS+10<DUTY_MAX){
            OC1RS+=10;
        }
    }
    else if(c=='d' || c=='D'){
        if(OC1RS-10>DUTY_MIN){
            OC1RS-=10;
        }
    }
    
    Ventana_LCD[1][15] = c;
    IFS1bits.U2RXIF = 0;
}

void _ISR_NO_PSV _U2TXInterrupt() {
    static int estado_uart = 0, i = 0;
    // Maquina de Estados UART2:
    switch(estado_uart){
        case 0:
            U2TXREG = home[i];
            i++;
            if (i==3){
                estado_uart=1;
                i=0;
            }  
            break;
        case 1:
            U2TXREG = Ventana_LCD[0][i];
            i++;
            if (i == 16) {
                estado_uart=2;
            i=0;
            }  
            break;
        case 2:
            U2TXREG = LF;
            estado_uart = 3;
            break;
        case 3:
            U2TXREG = CR;
            estado_uart = 4;
            break;
        case 4:
            U2TXREG = Ventana_LCD[1][i];
            i++;
            if (i == 16)  {
                estado_uart = 0;
                i=0;
            }
            break;
    }
    
    IFS1bits.U2TXIF = 0;
}