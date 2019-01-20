#ifndef _TECLADO_H_
#define _TECLADO_H_

#include "Global.h"

extern ESTADO_DIA estado_dia;
extern RENDERIZADO renderizado;
extern CAMARAS camara;
extern VEHICULOS vehiculos;


void onKey(unsigned char tecla, int x, int i) {
	//Callback de atencion al teclado

	switch (tecla) {

	case 'L':
	case 'l':
		if (estado_dia == DIA) {
			glClearColor(0, 0, 0, 1);  //fijar el color de borrado (fondo)
			glEnable(GL_LIGHTING);
			estado_dia = NOCHE;
		}
		else {
			glClearColor(1, 1, 1, 1);  //fijar el color de borrado (fondo)
			glDisable(GL_LIGHTING);
			estado_dia = DIA;
		}
		break;
	case 'S':
	case 's':
		if (renderizado == SOLIDO) {
			glClearColor(0, 0, 0, 1);  //fijar el color de borrado (fondo)
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			renderizado = ALAMBRICO;
		}
		else {
			//glClearColor(0, 0, 0, 1);  //fijar el color de borrado (fondo)
			glEnable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			renderizado = SOLIDO;
		}
		break;
	case 'X':
	case 'x':
		elementos_solidarios_camara_activos = false;
		if (camara == CAMARA_NORMAL) {
			camara = CAMARA_INTERIOR;
		}
		else if (camara == CAMARA_INTERIOR) {
			camara = CAMARA_VER_VEHICULO;
		}
		else {
			camara = CAMARA_NORMAL;
		}
		break;
	case 'V':
	case 'v':
		elementos_solidarios_camara_activos = false;
		if (vehiculo == TRACTOR) {
			vehiculo = MERCEDES;
		}
		else if (vehiculo == MERCEDES) {
			vehiculo = TRACTOR;
		}
		break;
	case 'C':
	case 'c':
		if (elementos_solidarios_camara_activos) {
			elementos_solidarios_camara_activos = false;
		}
		else {
			elementos_solidarios_camara_activos = true;
		}
		break;

	}
	//glutPostRedisplay(); // evento de redibujo con los cambios
}

void onSpecialKey(int tecla, int x, int y) {

	switch (tecla) {
	case GLUT_KEY_LEFT:
		angulo += 0.25;
		break;
	case GLUT_KEY_RIGHT:
		angulo -= 0.25;
		break;
	case GLUT_KEY_UP:
		modulo_velocidad += 0.1;
		break;
	case GLUT_KEY_DOWN:
		if (modulo_velocidad >= 0.1) {
			modulo_velocidad -= 0.1;
		}
		else {
			modulo_velocidad = 0;
		}
		break;
	}

	//glutPostRedisplay(); // REVISAR TECLA LANZA REDISPLAY
}


void muestra_comandos() {
	cout << "Teclas: " << endl;
	//cout << "\tA/a\t Activa o desactiva la luz ambiental." << endl;
	cout << "\tS/s\t Activa o desactiva Modo alámbrico." << endl;
	cout << "\tL/l\t Activa o desactiva Nocturno." << endl;
	//cout << "\tN/n\t Activa o desactiva la niebla." << endl;
	cout << "\tC/c\t Activa o desactiva elementos solidarios a la camara." << endl;
	cout << "\tv/V\t Cambia Vehiculo" << endl;
	cout << "\tx/X\t Cambia camara" << endl;
	//cout << "\tP/p\t Activa o desactiva parabrisas." << endl;
	//cout << "\tenter\t Activa o desactiva vista a�rea." << endl;
	//cout << "Cuando estamos en vista aeres, con la rueda del rat�n, hacemos zoom." << endl;
	//cout << "\tEsc\t Pausa/vuelve a iniciar." << endl;
	//cout << "\tespacio\t Saca foto del punto de vista." << endl;
}

#endif
