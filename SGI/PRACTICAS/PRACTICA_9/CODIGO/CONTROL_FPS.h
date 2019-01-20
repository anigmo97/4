#ifndef _CONTROL_FPS_H_
#define _CONTROL_FPS_H_

#include "Global.h"


extern float modulo_velocidad;
extern float vector_direccion[];
extern float angulo;
extern float x;
extern float z;

void FPS() {
	// cuenta fotogramas hasta que pasa 1 segundo
	// entonces los muestra y reinicia la cuenta

	int ahora, tiempo_transcurrido;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;
	// Cada vez que paso sumo un fotograma
	fotogramas++;

	//Calcular tiempo transcurrido 
	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempo_transcurrido = ahora - antes;

	// si ha transcurrido mas de un segundo muestro y reinicio
	if (tiempo_transcurrido > 1000) {
		stringstream titulo;
		titulo << PROYECTO << "   FPS = " << fotogramas;
		titulo << " VELOCIDAD = " << modulo_velocidad << " m/s";
		titulo << " ANGULO = " << angulo;
		titulo << " dir z= " << vector_direccion[Z_DIM];
		titulo << " dir x = " << vector_direccion[X_DIM];
		titulo << "x = " << x << "z = " << z;
		glutSetWindowTitle(titulo.str().c_str());
		fotogramas = 0;
		antes = ahora;
	}

}

#endif
