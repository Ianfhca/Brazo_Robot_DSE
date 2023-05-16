/*
 * Fichero: utilidades.c
 * Autores: Luis Castillo e Ian Fernandez
 * Descripcion: Funciones de utilidad general para las distintas conversiones
 * 
 */

// Definiciones necesarias
#include "p24HJ256GP610A.h"

unsigned char tabla_carac[16]="0123456789";

// Funcion para obtener los caracteres de un valor de dos digitos
void conversion_tiempo (unsigned char * dir, unsigned int val) {
    unsigned char dig;
    
    if (val > 99) {
        while(1);    
    } else {
        dig = val/10;
        dig = tabla_carac[dig];
        *dir = dig;
        dir++;
        dig = val%10;
        dig = tabla_carac[dig];
        *dir = dig;
    }
}

// Funcion para obtener los caracteres de un valor de cuatro digitos
void conversion_4digitos (unsigned char * dir, unsigned int val) {
    unsigned char dig;
    
    if (val > 9999) {
        while(1);    
    } else {
        dig = val/1000;
        dig = tabla_carac[dig];
        *dir = dig;
        dir++;
        dig = (val/100)%10;
        dig = tabla_carac[dig];
        *dir = dig;
        dir++;
        dig = (val/10)%10;
        dig = tabla_carac[dig];
        *dir = dig;
        dir++;
        dig = val%10;
        dig = tabla_carac[dig];
        *dir = dig;
    }
}

// Funcion para convertir de Byte a Int
unsigned int conversion_BytesToInt(unsigned char *dist) {
    return (unsigned int)(dist[0] << 8) | dist[1];
}