// Constantes y variables
#define DESV 100

extern int flag_muestras;
extern unsigned int tabla_Pot[8];
extern unsigned int tabla_Temp[8];
extern unsigned int tabla_Px[8];
extern unsigned int tabla_Py[8];
extern unsigned int tabla_Palanca[8];

// Funciones que se llaman desde otros modulos
void inic_ADC1();
void comienzo_muestreo();
void recoger_valor_ADC1();
void calcular_media_muestras();
void controlar_servos();