
// Constantes y variables
extern int flag_muestras;
extern unsigned int tabla_pot[8];
extern unsigned int tabla_temp[8];
extern unsigned int tabla_Px[8];
extern unsigned int tabla_Py[8];
extern unsigned int tabla_Palanca[8];
// Funciones que se llaman desde otros modulos
void inic_ADC1 ();
void comienzo_muestreo ();
void recoger_valorADC1 ();
void calcularMediaMuestras();

