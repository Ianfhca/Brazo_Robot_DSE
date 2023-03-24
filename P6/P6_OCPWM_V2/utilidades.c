#include "p24HJ256GP610A.h"

// Funciones para obtener los caracteres de un valor de dos digitos y de cuatro (conversion_adc)


unsigned char tabla_carac[16]="0123456789";
void conversion_tiempo (unsigned char * dir, unsigned int val)
{
    unsigned char dig;
    if (val>99) {
        while(1);    
    } else {
        dig=val/10;
        dig=tabla_carac[dig];
        *dir=dig;
        dir++;
        dig=val%10;
        dig=tabla_carac[dig];
        *dir=dig;
    }
}

void conversion_adc (unsigned char * dir, unsigned int val)
{
    unsigned char dig;
    if (val>9999) {
        while(1);    
    } else {
        dig=val/1000;
        dig=tabla_carac[dig];
        *dir=dig;
        dir++;
        dig=(val/100)%10;
        dig=tabla_carac[dig];
        *dir=dig;
        dir++;
        dig=(val/10)%10;
        dig=tabla_carac[dig];
        *dir=dig;
        dir++;
        dig=val%10;
        dig=tabla_carac[dig];
        *dir=dig;
    }
}
