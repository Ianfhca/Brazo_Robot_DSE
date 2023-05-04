#include "p24HJ256GP610A.h"
#include "memoria.h"


unsigned char pantalla[FILASPANTALLA][18] = {
    {"DSE 23 PROYECTO ""\x0D\x0A"}, // 1
    {"Ian F. y Luis C.""\x0D\x0A"}, // 2
    {"===INFO PIC24H==""\x0D\x0A"}, // 3
    {"CRONO: --:--:-- ""\x0D\x0A"}, // 4
    {"POTEN: ----     ""\x0D\x0A"}, // 5
    {"TEMPE: ----     ""\x0D\x0A"}, // 6
    {"====JOYSTICKS===""\x0D\x0A"}, // 7
    {"Px: ----        ""\x0D\x0A"}, // 8
    {"Py: ----        ""\x0D\x0A"}, // 9
    {"PL: ----        ""\x0D\x0A"}, // 10
    {"=====SERVOS=====""\x0D\x0A"}, // 11
    {"Servo 1: ----   ""\x0D\x0A"}, // 12
    {"Servo 2: ----   ""\x0D\x0A"}, // 13
    {"Servo 3: ----   ""\x0D\x0A"}, // 14
    {"Servo 4: ----   ""\x0D\x0A"}, // 15
    {"Servo 5: ----   ""\x0D\x0A"}  // 16
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