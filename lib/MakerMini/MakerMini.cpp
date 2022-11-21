#include "stdlib.h"
#include "arduino.h"
#include "MakerMini.h"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

Andromie::Andromie()
{
	_ULED = 13;
	_led[0] = 11;
	_led[1] = 10;
	_led[2] = 9;

	//Ultrasónico 2
	_trig = A4;
	_echo = A5;

	_SensorD1 = A6;
	_SensorD2 = A7;

	_servoInit = 0;

	_DireccionM1 = 7;
	_PwmM1 = 5;

	_DireccionM2 = 8;
	_PwmM2 = 6;

	_umbralProx = 250;
	_umbralPiso = 512;

	_tiempoFueraU = 11765; //Tiempo máximo para una lectura de 200 cm (Sensor Ultrasónico)
}

Andromie::Andromie(int velocidadDer, int velocidadIzq, int umbralProx, int umbralPiso)
{
	_ULED = 13;
	_led[0] = 11;
	_led[1] = 10;
	_led[2] = 9;

	_trig = A4;
	_echo = A5;

	_SensorD1 = A6;
	_SensorD2 = A7;

	_servoInit = 0;

	_DireccionM1 = 7;
	_PwmM1 = 5;

	_DireccionM2 = 8;
	_PwmM2 = 6;

	_umbralProx = umbralProx;
	_umbralPiso = umbralPiso;

	_tiempoFueraU = 11765; //Tiempo máximo para una lectura de 200 cm (Sensor Ultrasónico)
}

void Andromie::iniciar()
{
	pinMode(_DireccionM1,OUTPUT);
  	pinMode(_PwmM1,OUTPUT);

	pinMode(_DireccionM2,OUTPUT);
  	pinMode(_PwmM2,OUTPUT);

  	//Ultrasónico 1
 	pinMode(A2, OUTPUT);
  	pinMode(A3, INPUT); 	

  	//Ultrasónico 2
	pinMode(_trig, OUTPUT);
  	pinMode(_echo, INPUT);

	pinMode(_SensorD1, INPUT);
	pinMode(_SensorD2, INPUT);

	pinMode(_ULED, OUTPUT);
	pinMode(_led[0], OUTPUT);
	pinMode(_led[1], OUTPUT);
	pinMode(_led[2], OUTPUT);


}

void Andromie::motor(int motor, int direccion, int velocidad)
{
	_velocidad = velocidad;
	_direccion = direccion;

	if(motor == 1)
	{
		_velocidad = _velocidad;
		if(_direccion == 1)
		{
		digitalWrite(_DireccionM1, HIGH);
		//digitalWrite(_PwmM1, LOW);
		analogWrite(_PwmM1, 255 - _velocidad);
		}
		else if(_direccion == 0)
		{

		digitalWrite(_DireccionM1, LOW);
		//digitalWrite(_PwmM1, HIGH);
		analogWrite(_PwmM1, _velocidad);
		}
		}
	else if(motor == 2)
	{
		_velocidad = _velocidad;
		if(_direccion == 1)
		{
		digitalWrite(_DireccionM2, HIGH);
		analogWrite(_PwmM2, 255 - _velocidad);
		}
		else if(_direccion == 0)
		{
		digitalWrite(_DireccionM2, LOW);
		analogWrite(_PwmM2, _velocidad);
		}
	}
	
}


void Andromie::iniciarServos()
{
		servo1.attach(13);
		servo2.attach(12);
		servo3.attach(11);
}

void Andromie::servo(int pin, int ang)
{
	//Temporal mientras se agrega servoBegin();
	if(_servoInit == 0)
	{
		servo1.attach(13);
		servo2.attach(12);
		servo3.attach(11);
		_servoInit = 1;
	}
	
	_ang = ang;

	if(pin == 1)
	{
	servo1.write(_ang);
	}
	if(pin == 2)
	{
	servo2.write(_ang);
	}
	if(pin == 3)
	{
	servo3.write(_ang);
	}
}


void Andromie::userLED(boolean statLed)
{
	_statLed = statLed;
	digitalWrite(_ULED,_statLed);
}

void Andromie::LED(int led, boolean statLed)
{
	_statLed = statLed;
	digitalWrite(_led[led-1],_statLed);
}

void Andromie::LEDTricolor(int pwmRojo, int pwmVerde, int pwmAzul)
{
	_pwmRojo = map(pwmRojo, 0, 100, 0, 255);
	_pwmVerde = map(pwmVerde, 0, 100, 0, 255);
	_pwmAzul = map(pwmAzul, 0, 100, 0, 255);
	
	analogWrite(_led[0], _pwmRojo);
	analogWrite(_led[1], _pwmVerde);
	analogWrite(_led[2], _pwmAzul);
}

int Andromie::lecturaContacto()
{
	_tiempoFuera = 50;
	_tiempoAnterior = millis();
	_tiempoActual = millis();
	
	
	if(digitalRead(_SensorD1))
	{
		while(_tiempoActual < _tiempoAnterior + _tiempoFuera)
		{
			if(digitalRead(_SensorD2))
			{
				return 3;
			}
			_tiempoActual = millis();
		}
		return 1;
	}
	else if(digitalRead(_SensorD2))
	{
		while(_tiempoActual < _tiempoAnterior + _tiempoFuera)
		{
			if(digitalRead(_SensorD1))
			{
				return 3;
			}
			_tiempoActual = millis();
		}
		return 2;
	}
	else
	{
	return 0;
	}
}

float Andromie::lecturaSensorA(int entradaAnalogica, int parametro)
{
	_entradaAnalogica = entradaAnalogica + 5;
	_valorCAD = analogRead(_entradaAnalogica);
	if(parametro == 'V')
	{
		_valorCAD = (_valorCAD*5)/1023;
	}
	else if(parametro == '%')
	{
		_valorCAD = map(_valorCAD, 0, 1023, 0, 100);
	}
	else
	{
	}

	return _valorCAD;
}


long Andromie::lecturaDistancia(int IOUltra)
{
	_IOUltra = IOUltra;
	if(_IOUltra == 1)
	{
		_trig = A2;
		_echo = A3;
	}
	else if(_IOUltra == 2)
	{
		_trig = A4;
		_echo = A5;		
	}

	digitalWrite(_trig, LOW);
	delayMicroseconds(5);
	digitalWrite(_trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(_trig, LOW);
	_tiempoU = pulseIn(_echo, HIGH, _tiempoFueraU);
	if(_tiempoU == 0)
	{
		_tiempoU = _tiempoFueraU;
	}

	_distancia = _tiempoU*0.0172;
	return _distancia;
}

void Andromie::lecturaPiso()
{
	_pisoDer = analogRead(_pinSensorPisoDer);
	_pisoIzq = analogRead(_pinSensorPisoIzq);
}

bool Andromie::frenteEsObstaculo()
{
 	_lecturaContacto = lecturaContacto();
 	if(_lecturaContacto == 3)
 	{
   	return true;
 	}
 	else
 	{
   	return false;
 	}
}

bool Andromie::derechaEsObstaculo()
{
 	_lecturaContacto = lecturaContacto();
 	if(_lecturaContacto == 1)
 	{
   	return true;
 	}
 	else
 	{
   	return false;
 	}
}

bool Andromie::izquierdaEsObstaculo()
{
 	_lecturaContacto = lecturaContacto();
 	if(_lecturaContacto == 2)
 	{
   	return true;
 	}
 	else
 	{
   	return false;
 	}
}

bool Andromie::sensorContacto(int pinContacto)
{
	_pinContacto = pinContacto;

	if(pinContacto == 1)
	{
		if(digitalRead(_SensorD1))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
	else if(pinContacto == 2)
	{
		if(digitalRead(_SensorD2))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
	else
	{
		return false;
	}
}

bool Andromie::lecturaSensorD(int pin)
{	
	if(pin == 1)
	{
		if(analogRead(_SensorD1) > 512)
		{
		return true;
		}
		else
		{
		return false;
		}
	}
	else if(pin == 2)
	{
		if(analogRead(_SensorD2) > 512)
		{
		return true;
		}
		else
		{
		return false;
		}
	}
	else
	{
		return false;
	}
}

void Andromie::configurarSeguidor(boolean colorLinea, int umbralPiso)
{
	_pisoDer = A6;
	_pisoIzq = A7;
	_umbralPiso = umbralPiso;
	_colorLinea = colorLinea;
}

bool Andromie::frenteEsLinea()
{
	if(_colorLinea == true) //Linea blanca
	{
		if((analogRead(_pisoIzq) > _umbralPiso) && (analogRead(_pisoDer) > _umbralPiso))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
	else // Linea negra
	{
		if((analogRead(_pisoIzq) < _umbralPiso) && (analogRead(_pisoDer) < _umbralPiso))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
}

bool Andromie::izquierdaEsLinea()
{
	if(_colorLinea == true) //Linea blanca
	{
		if((analogRead(_pisoIzq) > _umbralPiso) && (analogRead(_pisoDer) < _umbralPiso))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
	else // Linea negra
	{
		if((analogRead(_pisoIzq) < _umbralPiso) && (analogRead(_pisoDer) > _umbralPiso))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
}

bool Andromie::derechaEsLinea()
{
	if(_colorLinea == true) //Linea blanca
	{
		if((analogRead(_pisoIzq) < _umbralPiso) && (analogRead(_pisoDer) > _umbralPiso))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
	else // Linea negra
	{
		if((analogRead(_pisoIzq) > _umbralPiso) && (analogRead(_pisoDer) < _umbralPiso))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
}