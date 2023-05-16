// Constantes y variables
#define D1 862
#define D2 1191
#define D3 1208
#define D4 445
#define D5 630
#define MULT 150
#define VEL 3
#define NMAXMOV 10

extern unsigned int DUTY_MIN[5];
extern unsigned int DUTY_MAX[5];
extern unsigned int DUTY[5];
extern unsigned int DEF_DUTY[5];
extern unsigned int duty_palanca;
extern unsigned int flag_servo;
extern unsigned int flag_objetivo;
extern unsigned int modo_control;
extern unsigned int servoActual;
extern unsigned int movActual;
extern unsigned int nMov;
extern unsigned int secuencia[NMAXMOV][5];

// Funciones que se llaman desde otros modulos
void mostrar_duty();
int relacion_adc_pwm(unsigned int valor_adc, int servo);