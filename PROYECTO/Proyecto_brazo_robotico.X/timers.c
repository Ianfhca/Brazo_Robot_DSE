/*
 * Fichero: timers.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Inicializacion de los timers y el trato correspondiente
 * a sus rutinas de atencion
 * 
 */

// Definiciones necesarias
#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "utilidades.h"
#include "PWM.h"
#include "LCD.h"

// Inicializacion de T8 con periodo de 20ms
void inic_Timer8(){
    TMR8 = 0;       // Inicializar el registro de cuenta
    PR8 = 12500;    // Periodo 20 milisegundos

    T8CONbits.TCKPS = 2;    // Escala del prescaler 1:64
    T8CONbits.TCS = 0;      // Reloj interno
    T8CONbits.TGATE = 0;    // Deshabilitar el modo Gate

    T8CONbits.TON = 1;      // Puesta en marcha del timer

    IEC3bits.T8IE = 1;
}

// Inicializacion de T7 con periodo de 10ms
void inic_Timer7() {
    TMR7 = 0;       // Inicializar el registro de cuenta
    PR7 = 50000;    // Periodo 10 milisegundos

    T7CONbits.TCKPS = 1;    // Escala del prescaler 1:8
    T7CONbits.TCS = 0;      // Reloj interno
    T7CONbits.TGATE = 0;    // Deshabilitar el modo Gate

    T7CONbits.TON = 1;      // Puesta en marcha del timer

    IEC3bits.T7IE = 1;
}

// Inicializacion de T5 con periodo de 2.5ms
void inic_Timer5() {
    TMR5 = 0;       // Inicializar el registro de cuenta
    PR5 = 12500;    // Periodo 2.5 milisegundos, MAX = 2^16 = 65.536

    T5CONbits.TCKPS = 1;    // Escala del prescaler 1:8
    T5CONbits.TCS = 0;      // Reloj interno
    T5CONbits.TGATE = 0;    // Deshabilitar el modo Gate

    T5CONbits.TON = 1;      // Puesta en marcha del timer

    IEC1bits.T5IE = 1;
}

// Inicializacion de T3 con periodo de 1ms
void inic_Timer3() {
    TMR3 = 0;       // Inicializar el registro de cuenta
    PR3 = 40000;    // Periodo 1 milisegundo

    T3CONbits.TCKPS = 0;    // Escala del prescaler 1:1
    T3CONbits.TCS = 0;      // Reloj interno
    T3CONbits.TGATE = 0;    // Deshabilitar el modo Gate

    T3CONbits.TON = 1;      // Puesta en marcha del timer

    IEC0bits.T3IE = 0;
}

// Inicializacion de T2 con periodo de 20ms
void inic_Timer2() {
    TMR2 = 0; // Inicializar el registro de cuenta
    PR2 = PR20ms;

    T2CONbits.TCKPS = 2; // escala del prescaler 1:64
    T2CONbits.TCS = 0; // reloj interno
    T2CONbits.TGATE = 0; // Deshabilitar el modo Gate

    T2CONbits.TON = 1; // puesta en marcha del timer

    IEC0bits.T2IE = 1;
}

// Inicializacion de las variables del cronometro
unsigned int mili, deci, seg, min;
int flag_inic_crono = 0;

void inic_crono()
{
    mili = 0;
    deci = 0;
    seg = 0;
    min = 0;

    flag_inic_crono = 1;
}

// Transfoma las variables de tiempo en caracteres imprimibles por la LCD
void comprobar_inic_crono() {
    if (flag_inic_crono == 1) {
        conversion_tiempo(&pantalla[3][13], deci);
        conversion_tiempo(&pantalla[3][7], min);
        conversion_tiempo(&pantalla[3][10], seg);

        flag_inic_crono = 0;
    }
}

// Establece el formato de tiempo
void crono() {
    if (mili == 100) {
        LATAbits.LATA2 = !LATAbits.LATA2;
        mili = 0;
        deci++;
        conversion_tiempo(&pantalla[3][13], deci);
    }
    if (deci == 10) {
        LATAbits.LATA0 = !LATAbits.LATA0;
        deci = 0;
        seg++;
        conversion_tiempo(&pantalla[3][10], seg);
    }
    if (seg == 60) {
        seg = 0;
        min++;
        conversion_tiempo(&pantalla[3][7], min);
    }
}

// Rutina de atencion del T8
// Suaviza el movimiento de los servos
// Controla que el brazo vuelva a una posicion segura al finalizar
void _ISR_NO_PSV _T8Interrupt() {
    static int ini[5] = {0, 0, 0, 0, 0};
    
    if (DUTY[2]<duty_palanca && modo_control==1) {
        if (DUTY[2]+10>=duty_palanca){
            DUTY[2] = duty_palanca;
        } else {
            DUTY[2]+=10;
        }
    } else if (DUTY[2]>duty_palanca && modo_control==1) {
        if (DUTY[2]-10<=duty_palanca){
            DUTY[2] = duty_palanca;
        } else {
            DUTY[2]-=10;
        }
    }
    
    if(flag_gatillo == 1) {
        if (DUTY[4]+10 < DUTY_MAX[4]) {
            DUTY[4] += 10;
        } else {
            DUTY[4] = DUTY_MAX[4];
        }
    }
    
    if (flag_derecho == 1) {
        if (DUTY[4]-10 > DUTY_MIN[4]) {
            DUTY[4] -= 10;
        } else {
            DUTY[4] = DUTY_MIN[4];
        }
    }
            
    
    if (flag_ini == 1) {
        if (DUTY[0]+5 < D1) {
            DUTY[0] += 5;
        } else if (DUTY[0]-5 > D1) {
            DUTY[0] -= 5;
        } else {
            DUTY[0] = D1;
            ini[0] = 1;
        }
        
        if (DUTY[1]+5 < D2) {
            DUTY[1] += 5;
        } else if (DUTY[1]-5 > D2) {
            DUTY[1] -= 5;
        } else {
            DUTY[1] = D2;
            ini[1] = 1;
        }
        
        if (DUTY[2]+5 < D3) {
            DUTY[2] += 5;
        } else if (DUTY[2]-5 > D3) {
            DUTY[2] -= 5;
        } else {
            DUTY[2] = D3;
            ini[2] = 1;
        }
        
        if (DUTY[3]+5 < D4) {
            DUTY[3] += 5;
        } else if (DUTY[3]-5 > D4) {
            DUTY[3] -= 5;
        } else {
            DUTY[3] = D4;
            ini[3] = 1;
        }
        
        if (DUTY[4]+5 < D5) {
            DUTY[4] += 5;
        } else if (DUTY[4]-5 > D5) {
            DUTY[4] -= 5;
        } else {
            DUTY[4] = D5;
            ini[4] = 1;
        }
        
        if (ini[0] && ini[1] && ini[2] && ini[3] && ini[4]) {
            ini[0] = 0;
            ini[1] = 0;
            ini[2] = 0;
            ini[3] = 0;
            ini[4] = 0;
            
            flag_ini = 0;
        }
    }
    
    if (flag_sec == 1 && flag_ini == 0 && flag_grabado==1) {
        if (DUTY[0]+5 < secuencia[movActual][0]) {
            DUTY[0] += 5;
        } else if (DUTY[0]-5 > secuencia[movActual][0]) {
            DUTY[0] -= 5;
        } else {
            DUTY[0] = secuencia[movActual][0];
            ini[0] = 1;
        }
        
        if (DUTY[1]+5 < secuencia[movActual][1]) {
            DUTY[1] += 5;
        } else if (DUTY[1]-5 > secuencia[movActual][1]) {
            DUTY[1] -= 5;
        } else {
            DUTY[1] = secuencia[movActual][1];
            ini[1] = 1;
        }
        
        if (DUTY[2]+5 < secuencia[movActual][2]) {
            DUTY[2] += 5;
        } else if (DUTY[2]-5 > secuencia[movActual][2]) {
            DUTY[2] -= 5;
        } else {
            DUTY[2] = secuencia[movActual][2];
            ini[2] = 1;
        }
        
        if (DUTY[3]+5 < secuencia[movActual][3]) {
            DUTY[3] += 5;
        } else if (DUTY[3]-5 > secuencia[movActual][3]) {
            DUTY[3] -= 5;
        } else {
            DUTY[3] = secuencia[movActual][3];
            ini[3] = 1;
        }
        
        if (DUTY[4]+5 < secuencia[movActual][4]) {
            DUTY[4] += 5;
        } else if (DUTY[4]-5 > secuencia[movActual][4]) {
            DUTY[4] -= 5;
        } else {
            DUTY[4] = secuencia[movActual][4];
            ini[4] = 1;
        }
        
        if (ini[0] && ini[1] && ini[2] && ini[3] && ini[4]) {
            ini[0] = 0;
            ini[1] = 0;
            ini[2] = 0;
            ini[3] = 0;
            ini[4] = 0;
            
            movActual++;
            
            if (movActual==nMov){
                flag_sec = 0;
                movActual = 0;
            }
        }
    }
    
    IFS3bits.T8IF = 0;
}

// Rutina de atencion del T8
// Control del tiempo espera 10 ms y luego actualiza
void _ISR_NO_PSV _T7Interrupt() {
    mili += 10;
    IFS3bits.T7IF = 0;
}

// Rutina de atencion del T8
// Controla el refresco de la LCD
void _ISR_NO_PSV _T5Interrupt() {
    static int estado_lcd = 0, i = 0;
    // Maquina de Estados:
    switch (estado_lcd) {
        case 0:
            i = 0;
            lcd_cmd(0x80);
            estado_lcd = 1;
            break;
        case 1:
            lcd_data(Ventana_LCD[0][i]);
            i++;
            if (i == 16)
                estado_lcd = 2;
            break;
        case 2:
            i = 0;
            lcd_cmd(0xC0);
            estado_lcd = 3;
            break;
        case 3:
            lcd_data(Ventana_LCD[1][i]);
            i++;
            if (i == 16)
                estado_lcd = 0;
            break;
    }
    IFS1bits.T5IF = 0;
}

// Rutina de atencion del T2
// Durante 20ms calcula cantidad de tiempo que se activa cada servo
void _ISR_NO_PSV _T2Interrupt() {
    static int servo = 0, aux; 

    switch(servo) {
        case 0:
            PR2 = DUTY[servo];
            LATDbits.LATD0 = 1;
            aux = PR20ms - PR2;
            servo++;
            break;
        case 1:
            PR2 = DUTY[servo];
            LATDbits.LATD0 = 0;
            LATDbits.LATD1 = 1;
            aux = aux - PR2;
            servo++;
            break;
        case 2:
            PR2 = DUTY[servo];
            LATDbits.LATD1 = 0;
            LATDbits.LATD2 = 1;
            aux = aux - PR2;
            servo++;
            break;
        case 3:
            PR2 = DUTY[servo];
            LATDbits.LATD2 = 0;
            LATDbits.LATD3 = 1;
            aux = aux - PR2;
            servo++;
            break;
        case 4:
            PR2 = DUTY[servo];
            LATDbits.LATD3 = 0;
            LATDbits.LATD8 = 1;
            aux = aux - PR2;
            servo++;
            break;
        case 5:
            PR2 = aux;
            LATDbits.LATD8 = 0;
            servo = 0;
            break;
    }
    
    flag_servo = 1;
    IFS0bits.T2IF = 0;
}

// Realiza una espera de la cantidad de milisegundos pasados por parametro
void Delay_ms(int milisegundos) {
    TMR9 = 0;   // Inicializar el registro de cuenta
    unsigned long ciclos = (unsigned long) 40000 * milisegundos;
    unsigned long max_ciclos = 65536;
    unsigned long prescaler = ciclos / max_ciclos;
    if (prescaler < 1) {
        PR9 = ciclos;           // Periodo del timer 40000 * milisegundos
        T9CONbits.TCKPS = 0;    // Escala del prescaler 1:1
    } else if (prescaler < 8) {
        PR9 = ciclos / 8;       // Periodo del timer 40000 * milisegundos / 8
        T9CONbits.TCKPS = 1;    // Escala del prescaler 1:8
    } else if (prescaler < 64) {
        PR9 = ciclos / 64;      // Periodo del timer 40000 * milisegundos / 64
        T9CONbits.TCKPS = 2;    // Escala del prescaler 1:64
    } else if (prescaler < 256) {
        PR9 = ciclos / 256;     // Periodo del timer 40000 * milisegundos / 256
        T9CONbits.TCKPS = 3;    // Escala del prescaler 1:256
    }
    T9CONbits.TCS = 0;      // Reloj interno
    T9CONbits.TGATE = 0;    // Deshabilitar el modo Gate

    IFS3bits.T9IF = 0;

    T9CONbits.TON = 1;      // Puesta en marcha del timer    

    // Esperar hasta que cuente los milisegundos
    while (!IFS3bits.T9IF) { 
        // Se activa IFS3bits.T9IF = 1;
    }
    IFS3bits.T9IF = 0;
    T9CONbits.TON = 0;
}

// Realiza una espera de la cantidad de microsegundos pasados por parametro
void Delay_us(int microsegundos) {
    TMR9 = 0;   // Inicializar el registro de cuenta
    unsigned long ciclos = (unsigned long) 40 * microsegundos;
    unsigned long max_ciclos = 65536;
    unsigned long prescaler = ciclos / max_ciclos;
    if (prescaler < 1) {
        PR9 = ciclos;           // Periodo del timer 40000 * microsegundos
        T9CONbits.TCKPS = 0;    // Escala del prescaler 1:1
    } else if (prescaler < 8) {
        PR9 = ciclos / 8;       // Periodo del timer 40000 * microsegundos / 8
        T9CONbits.TCKPS = 1;    // Escala del prescaler 1:8
    } else if (prescaler < 64) {
        PR9 = ciclos / 64;      // Periodo del timer 40000 * microsegundos / 64
        T9CONbits.TCKPS = 2;    // Escala del prescaler 1:64
    } else if (prescaler < 256) {
        PR9 = ciclos / 256;     // Periodo del timer 40000 * microsegundos / 256
        T9CONbits.TCKPS = 3;    // Escala del prescaler 1:256
    }
    T9CONbits.TCS = 0;      // Reloj interno
    T9CONbits.TGATE = 0;    // Deshabilitar el modo Gate

    IFS3bits.T9IF = 0;

    T9CONbits.TON = 1;      // puesta en marcha del timer

    // Esperar hasta que cuente los microsegundos
    while (!IFS3bits.T9IF) { 
        // Se activa IFS3bits.T9IF = 1;
    }
    IFS3bits.T9IF = 0;
    T9CONbits.TON = 0;
}