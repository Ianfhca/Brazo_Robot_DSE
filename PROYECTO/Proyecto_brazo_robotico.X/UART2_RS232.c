/* Funciones para trabajar con el modulo UART2
================================================
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "UART2_RS232.h"
#include "timers.h"
#include "memoria.h"



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
    switch (c){
    case 'p':
        T7CONbits.TON = 0;
        break;
    case 'i':
        inic_crono();
        break;
    case 'c':
        T7CONbits.TON = 1;
        break;
    }

    IFS1bits.U2RXIF = 0;
}

void _ISR_NO_PSV _U2TXInterrupt() {
    static int estado_uart = 0, i = 0, j=0;
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
            U2TXREG = pantalla[j][i];
            i++;
            if (i == 18) {
                i=0;
                j++;
            }
            
            if(j==16){
                estado_uart = 0;
                j=0;
            }
            break;
        
    }
    
    IFS1bits.U2TXIF = 0;
}