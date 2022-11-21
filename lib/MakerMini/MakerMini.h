#ifndef MakerMini_h
#define MakerMini_h
#include "arduino.h"
#include <Servo.h>

class Andromie
{
public:
	Andromie();
	Andromie(int speedDer, int speedIzq, int umbralProx, int umbralPiso);
	void iniciar();

	//Funciones para motres y servos
	void motor(int motor, int direccion, int velocidad);
	void servo(int pin, int ang);
	void iniciarServos();

	//Funciones para Salidas LEDs
	void userLED(boolean statLed);
	void LED(int pin, boolean statLed);
	void LEDTricolor(int pwmRojo, int pwmVerde, int pwmAzul);

	//Funciones para lectura de entradas y sensores
	bool lecturaSensorD(int pin);
	float lecturaSensorA(int entradaAnalogica, int parametro);
	long lecturaDistancia(int IOUltra);

	//Funciones para robot evade obstáculos con bumpers
	bool sensorContacto(int pinContacto);
	int lecturaContacto();
	bool frenteEsObstaculo();
	bool derechaEsObstaculo();
	bool izquierdaEsObstaculo();
	
	//Funciones para seguidor de línea
	void lecturaPiso();
	void configurarSeguidor(boolean colorLinea, int umbralPiso);
	bool frenteEsLinea();
	bool izquierdaEsLinea();
	bool derechaEsLinea();

private:
	int _led[4];

	int _SensorD1;
	int _SensorD2;

	int _IOUltra;

	int _trig;
	int _echo;

	int _servoInit;

	int _pwmRojo;
	int _pwmVerde;
	int _pwmAzul;

	int _motor;
	int _direccion;
	int _velocidad;

	int _ang;

	int _pinContacto;

	int _DireccionM1;
	int _PwmM1;

	int _DireccionM2;
	int _PwmM2;

	int _entradaAnalogica;
	int _valorPot;
	float _valorCAD;
	
	int _ULED;
	
	boolean _statLed1;
	boolean _statLed2;
	boolean _statLed3;
	boolean _statLed4;
	boolean _statLed5;
	boolean _statLed6;
	boolean _statLed7;
	boolean _statLed;

	int _pisoDer;
	int _pisoIzq;

	boolean _colorLinea;

	int _pinSensorPisoDer;
	int _pinSensorPisoIzq;
	
	int _umbralProx;
	int _umbralPiso;

	int _tiempoFuera;
	double _tiempoAnterior;
	double _tiempoActual;

	int _lecturaContacto;

	//Variables para sensor ultrasónico
	long _tiempoFueraU;
	long _tiempoU;
	long _distancia;
};

#endif