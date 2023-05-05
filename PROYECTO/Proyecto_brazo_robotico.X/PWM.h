// Variables y funciones exportadas para utilizarlas desde otros modulos

// VARIABLES
//=========================================================
extern unsigned int DUTY_MIN;
extern unsigned int DUTY_MAX;
extern unsigned int DUTY[5];
extern unsigned int flag_servo;
extern unsigned int modo_control;
extern unsigned int servoActual;

// FUNCIONES
//=========================================================
void mostrar_duty();
int relacion_adc_pwm(unsigned int valor_adc);
