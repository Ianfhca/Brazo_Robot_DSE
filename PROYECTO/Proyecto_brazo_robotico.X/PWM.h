// Variables y funciones exportadas para utilizarlas desde otros modulos

#define MULT 150
#define VEL 5

// VARIABLES
//=========================================================
extern unsigned int DUTY_MIN[5];
extern unsigned int DUTY_MAX[5];
extern unsigned int DUTY[5];
extern unsigned int duty_palanca;
extern unsigned int flag_servo;
extern unsigned int flag_objetivo;
extern unsigned int modo_control;
extern unsigned int servoActual;

// FUNCIONES
//=========================================================
void mostrar_duty();
int relacion_adc_pwm(unsigned int valor_adc, int servo);
void actualizarDuty();
