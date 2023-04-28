#include "p24HJ256GP610A.h"
#include "memoria.h"


unsigned char pantalla[FILASPANTALLA][18] = {
    {"DSE 23 PROYECTO ""\x0D\x0A"},
    {"Ian F. y Luis C.""\x0D\x0A"},
    {"===INFO PIC24H==""\x0D\x0A"},
    {"CRONO: --:--:-- ""\x0D\x0A"},
    {"POTEN: ----     ""\x0D\x0A"},
    {"TEMPE: ----     ""\x0D\x0A"},
    {"====JOYSTICKS===""\x0D\x0A"},
    {"Px: ----        ""\x0D\x0A"},
    {"Py: ----        ""\x0D\x0A"},
    {"PL: ----        ""\x0D\x0A"},
    {"=====SERVOS=====""\x0D\x0A"},
    {"Servo 1: ----   ""\x0D\x0A"},
    {"Servo 2: ----   ""\x0D\x0A"},
    {"Servo 3: ----   ""\x0D\x0A"},
    {"Servo 4: ----   ""\x0D\x0A"},
    {"Servo 5: ----   ""\x0D\x0A"}
};

unsigned char Ventana_LCD[2][16] = {
    {"DSE 23 PROYECTO "},
    {"--- PULSA S3 ---"}
};


int indice = 1, flagScroll=-1;

void scrollLCD(int accion){
    //Accion define si el scroll es hacia arriba o hacia abajo
    static int k = 0;
    
    if(accion==1 && indice<FILASPANTALLA-1){
        for(k=0; k<16; k++){
            Ventana_LCD[0][k] = pantalla[indice][k];
            Ventana_LCD[1][k] = pantalla[indice+1][k];
        }
        indice++;
    }
        
    else if(accion==0 && indice>0){
        for(k=0; k<16; k++){
            Ventana_LCD[0][k] = pantalla[indice-1][k];
            Ventana_LCD[1][k] = pantalla[indice][k];
        }
        indice--;
    }
        
}

void actualizarLCD(){
    int k = 0;
    
    for(k=0; k<16; k++){
        Ventana_LCD[0][k] = pantalla[indice-1][k];
        Ventana_LCD[1][k] = pantalla[indice][k];
    }
    
}