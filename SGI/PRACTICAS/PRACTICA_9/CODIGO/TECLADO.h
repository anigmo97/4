#ifndef _TECLADO_H_
#define _TECLADO_H_

#include "Global.h"

extern ESTADO_DIA estado_dia;
extern RENDERIZADO renderizado;
extern CAMARAS camara;
extern VEHICULOS vehiculo;
extern bool camara_aerea;
extern bool pausa;
extern int contador_simbolo_pausa;
extern bool elementos_solidarios_camara_activos;
extern bool niebla_activada;

int num_capturas = 1;

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
		camara_aerea = false;
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
		camara_aerea = false;
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
		camara_aerea = false;
		if (elementos_solidarios_camara_activos) {
			elementos_solidarios_camara_activos = false;
		}
		else {
			elementos_solidarios_camara_activos = true;
		}
		break;
	case 'N':
	case 'n':
		if (niebla_activada) {
			glDisable(GL_FOG);
			niebla_activada = false;
		}
		else {
			niebla_activada = true;
		}
		break;
	case 'A':
	case 'a':
		camara = CAMARA_NORMAL;
		camara_aerea = !camara_aerea;
		break;
	case 'P':
	case 'p':
		pausa = !pausa;
		contador_simbolo_pausa = 0;
		break;
	case 'W':
	case 'w':
		GLint vector_captura[4];
		glGetIntegerv(GL_VIEWPORT, vector_captura);
		string nombre_captura = "screenShot" + std::to_string(num_capturas)+ ".png";
		saveScreenshot(&nombre_captura[0u], vector_captura[2], vector_captura[3]);
		num_capturas++;
		break;
	}


	//glutPostRedisplay(); // evento de redibujo con los cambios
}

void onSpecialKey(int tecla, int x, int y) {
	if (!pausa) {
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
	}
}


void muestra_comandos() {
	cout << "Teclas: " << endl;
	cout << "\t S/s \t Activa o desactiva Modo alambrico." << endl;
	cout << "\t L/l \t Activa o desactiva Nocturno." << endl;
	cout << "\t N/n \t Activa o desactiva la niebla." << endl;
	cout << "\t C/c \t Activa o desactiva elementos solidarios a la camara." << endl;
	cout << "\t v/V \t Cambia Vehiculo" << endl;
	cout << "\t x/X \t Cambia camara" << endl;
	cout << "\t A/a \t Activa o desactiva vista aérea.(zoom -> rueda raton)" << endl;
	cout << "\t P/p \t Pausa/Continuar." << endl;
	cout << "\t W/w \t Captura de pantalla" << endl;
}

#endif
