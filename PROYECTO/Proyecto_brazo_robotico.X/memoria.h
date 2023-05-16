// Constantes y variables
#define FILASPANTALLA 16

extern unsigned char pantalla[16][18];
extern unsigned char Ventana_LCD[2][16];
extern int indice;
extern int flag_scroll;

// Funciones que se llaman desde otros modulos
void scroll_LCD(int accion);
void actualizar_LCD();
