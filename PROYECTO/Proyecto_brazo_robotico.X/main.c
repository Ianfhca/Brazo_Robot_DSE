/*
 * Fichero: main.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Fichero principal del proyecto
 * 
 */

// Definiciones necesarias
#include "p24HJ256GP610A.h"
#include "oscilator.h"
#include "GPIO.h"
#include "timers.h"
#include "commons.h"
#include "LCD.h"
#include "memoria.h"
#include "UART2_RS232.h"
#include "utilidades.h"
#include "ADC1.h"
#include "PWM.h"

int main(void) {
    // INICIALIZACION PRE-ARRANQUE DEL PROGRAMA
    inic_oscilator();  // Inicializacion del reloj del sistema: 80 MHz
	inic_leds();	   // Inicializacion de los leds
    inic_pulsadores(); // Inicializacion de pulsadores
    inic_servos();     // Inicializacion de los servomotores
    inic_LCD();        // Inicializacion de la pantalla LCD
    
    // Establecimiento de los valores iniciales de la ventana LCD
    line_1();    
    puts_lcd(Ventana_LCD[0], 16);
    line_2();
    puts_lcd(Ventana_LCD[1], 16);
    
    // El programa arranca al pulsar el boton S3
    while(PORTDbits.RD6){
        
    }
    
    // INICIALIZACION POST-ARRANQUE DEL PROGRAMA
    scroll_LCD(0);     // Inicializa el scroll de la LCD en la posicion inicial
    inic_crono();	  // Inicializacion de las variables del cronometro
	inic_Timer7();	  // Inicializacion de T7 con periodo de 10ms
    inic_Timer5();    // Inicializacion de T5 con periodo de 2.5ms
    inic_CN();        // Inicializacion de interrupciones para los pulsadores
    inic_UART2();     // Inicializacion de la UART
    U2TXREG = 0;      // Bit nulo para comenzar la comunicacion
    inic_Timer3();    // Inicializacion de T3 con periodo de 1ms
    inic_ADC1();      // Inicializacion del ADC1
    inic_servos();    // Inicializacion de los servos
    inic_Timer2();    // Inicializacion de T2 con periodo de 20ms
    mostrar_duty();   // Muestra los 5 DUTYs por pantalla
    inic_Timer8();    // Inicializacion de T8 con periodo de 20ms
    
    // BUCLE PRINCIPAL DEL PROGRAMA
    while(1){
        // Se actualiza el cronometro y se comprueba si se para/reinicia
        crono();     
        comprobar_inic_crono();
        
        // Se controla el scroll de la LCD mediante 2 botones
        if(flag_scroll == 0){
            scroll_LCD(0);
            flag_scroll = -1;
        } else if(flag_scroll == 1){
            scroll_LCD(1);
            flag_scroll = -1;
        }
        
        // Se actualiza la LCD con frequencia si no esta situada en la cabecera
        if(indice > 1)
            actualizar_LCD();
        
        // Tras recoger las 8 muestras de cada dispositivo se realiza la media
        // de los datos recogidos y se actualiza la informacion
        if (flag_muestras == 1) {
            calcular_media_muestras(); 
            flag_muestras = 0;
        }
        
        // Tras 20ms se muestra por pantalla el valor de todos los DUTY
        if(flag_servo){
            mostrar_duty();
            flag_servo = 0;
        }
        
        // Determina si el brazo se controla mediante la UART o el Joystick
        if(modo_control)
            controlar_servos();        
    }
    
    return 0;
}
