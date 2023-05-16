// Constantes y variables
#define Nop()   __builtin_nop()

#define _ISR_NO_PSV __attribute__((interrupt, no_auto_psv))

/////////////////////////////////////
// Definiciones para la pantalla LCD
/////////////////////////////////////

// Senial de control para los pines de datos
#define  RW  LATDbits.LATD5       // Senial LCD R/W
#define  RS  LATBbits.LATB15      // Senial LCD RS
#define  E   LATDbits.LATD4       // Senial LCD E 

// Senial de control para los pines de direccion 
#define  RW_TRIS	TRISDbits.TRISD5 
#define  RS_TRIS	TRISBbits.TRISB15
#define  E_TRIS		TRISDbits.TRISD4

// Seniales de datos y pines de direccion
#define  DATA      LATE           // Puerto para los datos del LCD
#define  DATAPORT  PORTE
#define  TRISDATA  TRISE          // E/S para el puerto de datos


//////////////////////////////////////
// Definiciones posiciones cronometro
//////////////////////////////////////

#define posds 13
#define posseg 10
#define posmin 7

///////////////////////////////////////////////////////////////
// Definiciones ligadas a la velocidad de transmision de UART2
///////////////////////////////////////////////////////////////

#define Fosc 80000000       // Frecuencia de reloj de la CPU (oscillator)
#define Fcy	 Fosc/2         // Velocidad de ejecucion de las instrucciones
#define BAUDRATE2 38400     // Velocidad de transmision de UART2 (baudios)
#define T_1BIT_US (1000000/BAUDRATE2)+1 // Duracion de 1 bit 

#define BAUD_RATEREG_2_BRGH1 ((Fcy/BAUDRATE2)/4)-1  // Valor U2BRG si BRGH=1
#define BAUD_RATEREG_2_BRGH0 ((Fcy/BAUDRATE2)/16)-1 // Valor U2BRG si BRGH=0

/////////////////////////////////////////////////////////////////
// Definiciones relacionadas con la pantalla del terminal del PC
/////////////////////////////////////////////////////////////////

#define clrscr "\x1b[2J" //4 character: \x1b, [, 2, J
#define home "\x1b[H"    //3 character: \x1b, [, H

#define CR 0x0D		// Carriage return
#define LF 0x0A		// Line feed


//////////////////////////////////////////////////
// Definiciones relacionadas con los servos y PWM
//////////////////////////////////////////////////

#define PR20ms  12500 	// Periodo adecuado para conseguir 20 ms
                        // teniendo en cuenta un prescaler de 1:64.
                        // Fosc= 80MHz Fcy= 40MHz Tcy=25ns
#define MINPWM 0.5      // Duracion minima pulso PWM (en milisegundos)
#define MAXPWM 2.1      // Duracion maxima pulso PWM (en milisegundos)

////////////////////////////
// Variables para las flags
////////////////////////////

extern int flag_gatillo;
extern int flag_ini;
extern int flag_derecho;
extern int flag_sec;
extern int flag_grabar;
extern int flag_grabado;

// Funciones que se llaman desde otros modulos
void inic_CN();