/*
 * Fichero: LCD.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Funciones para el funcionamiento del LCD
 * 
 */

// Definiciones necesarias
#include "p24HJ256GP610A.h"
#include "commons.h"
#include "timers.h"
#include "LCD.h"
#include "memoria.h"
#include "utilidades.h"

// Subrrutinas para los comandos del LCD
void lcd_cmd(char cmd) {
  RW = 0;            // RW=0, para escribir
  RS = 0;	         // RS=0, se va a escribir un comando
  DATA &= 0xFF00;    // Pines RD0-RD7 (Dato que se enviara): Poner a 0
  DATA |= cmd;       // Copiar a esos pines el parametro cmd
  E = 1;             // E=1. Debe permanecer activado al menos 230 ns. 
  // Delay necesario
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  E = 0;	// Desactivar E
  RW = 1;	// Desactivar escritura
}

// Subrrutina para los datos del LCD
void lcd_data(char data) {
  RW = 0;            // RW=0, para escribir
  RS = 1;            // RS=1, se va a escribir un caracter
  DATA &= 0xFF00;    // pines RD0-RD7 (dato que se envía): poner a 0
  DATA |= data;      // copiar a esos pines el parametro data
  E = 1;             // E=1. Debe permanecer activado al menos 230 ns.
  // Delay necesario
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  E = 0;    // Desactivar E
  RS = 0;   // Desactivar RS
  RW = 1;	// Desactivar escritura
}
// Inicializacion de la pantalla LCD
// 15ms de delay despues de que Vdd llegue a nnVdc antes de proceder
// No se requiere siempre y esta basado en el sistema Vdd rise rate
void inic_LCD() {
  delay_ms (15);    // Delay de 15ms
  
  // Establecer el estado inicial para los pines de datos y control
  DATA &= 0xFF00;	
  RW = 0;           // Estado R/W en low
  RS = 0;           // Estado RS en low
  E = 0;            // Estado E low

  /* set data and control pins to outputs */
  TRISDATA &= 0xFF00;
  RW_TRIS = 0;      // RW pin set as output
  RS_TRIS = 0;      // RS pin set as output
  E_TRIS = 0;       // E pin set as output

  /* LCD initialization sequence */ 
  lcd_cmd (0x38);	// Funcion set (3 veces)
  delay_ms (5);     // Delay de 5ms
  lcd_cmd (0x38);
  delay_us (100);   // Delay de 100us
  lcd_cmd (0x38);
  delay_us (40);    // Delay de 40us
  lcd_cmd (0x38);  
  delay_us (40);    // Delay de 40us
  lcd_cmd (0x0C);   // Desplegar el control, cursor blink off (0x0C)
  delay_us (40);    // Delay de 40us
  lcd_cmd (0x06);	// Establecer entry mode en (0x06)
  delay_us (40);    // Delay de 40us
}

// Escribe los caracteres pasados por parametro en la LCD
void puts_lcd(unsigned char *data, unsigned char count) {
  while(count) {
	 lcd_data(*data++);
         delay_us (40); // Delay de 40us
	 count--;
	}	
}

// Posicionarse en la primera linea del LCD
void line_1() {
    lcd_cmd(0x80);  // Establecer la memoria DDRAM a (@0)
    delay_us (40);  // Delay de 40us
}

// Posicionarse en la segunda linea del LCD
void line_2() {
    lcd_cmd(0xC0);   // Establecer la memoria DDRAM a (@40)
    delay_us (40);   // Delay de 40us
}