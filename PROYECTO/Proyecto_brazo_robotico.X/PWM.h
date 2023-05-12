// Variables y funciones exportadas para utilizarlas desde otros modulos
#define D1 862
#define D2 1191
#define D3 1208
#define D4 445
#define D5 630
#define MULT 150
#define VEL 5


// VARIABLES
//=========================================================
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
extern unsigned int secuencia[5][5];

// FUNCIONES
//=========================================================
void mostrar_duty();
int relacion_adc_pwm(unsigned int valor_adc, int servo);
void actualizarDuty();
