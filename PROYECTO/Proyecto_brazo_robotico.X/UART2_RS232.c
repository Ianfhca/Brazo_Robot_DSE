/* Funciones para trabajar con el modulo UART2
================================================
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "UART2_RS232.h"
#include "timers.h"
#include "memoria.h"
#include "PWM.h"

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


int flag_calibracion = 0, flag_grabar = 0, flag_grabado = 0, flag_sec = 0;
void _ISR_NO_PSV _U2RXInterrupt() {
    char c = U2RXREG;
    switch (c){
    case 'p': case 'P':
        T7CONbits.TON = 0;
        break;
    case 'i': case 'I':
        inic_crono();
        break;
    case 'c': case 'C':
        T7CONbits.TON = 1;
        break;
    case 's': case 'S':
        if(DUTY[servoActual]+10<=DUTY_MAX[servoActual] && modo_control==0){
            DUTY[servoActual]+=10;
            //flag_servo = 1;
        }
        break;
        
    case 'd': case 'D':  
        if(DUTY[servoActual]-10>=DUTY_MIN[servoActual] && modo_control==0){
            DUTY[servoActual]-=10;
            //flag_servo = 1;
        }
        break;    
    case 'm': case 'M':
        modo_control = !modo_control;
        break;
    
    case 'q': case 'Q':
        flag_calibracion = 1;
        DUTY_MIN[servoActual] = 312;
        DUTY_MAX[servoActual] = 1312;
        break;    
    
    case 'e': case 'E':
        if(flag_calibracion){
            DUTY_MIN[servoActual] = DUTY[servoActual];
        }
        break;    
    
    case 'r': case 'R':
        if(flag_calibracion){
            DUTY_MAX[servoActual] = DUTY[servoActual];
        }
        break;
        
    case 'w': case 'W':
        flag_calibracion = 0;
        break;
    
    case 'f': case 'F':
        flag_grabar = 1;
        movActual = 0;
        nMov = 0;
    break;
    
    case 'g': case 'G':
        if(flag_grabar){
            secuencia[movActual][0] = DUTY[0];
            secuencia[movActual][1] = DUTY[1];
            secuencia[movActual][2] = DUTY[2];
            secuencia[movActual][3] = DUTY[3];
            secuencia[movActual][4] = DUTY[4];
            
            movActual++;
            nMov++;
            
            if(movActual==NMAXMOV){
                flag_grabar = 0;
                movActual = 0;
                flag_grabado = 1;
                flag_ini = 1;
                modo_control = 0;
            }
        }
    break;
    
    case 'h': case'H':
        flag_grabar = 0;
        movActual = 0;
        flag_grabado = 1;
        flag_ini = 1;
        modo_control = 0;
        break;
    
    case 'j': case 'J':
        if(flag_sec==0){
            flag_ini = 1;
            modo_control = 0;
            flag_sec = 1;
        }
        
    break;
    
    case '1': case '2': case '3': case '4': case '5':
        servoActual = c - '0';
        servoActual--;
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