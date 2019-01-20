#ifndef _TEXTURAS_VEHICULOS_H_
#define _TEXTURAS_VEHICULOS_H_

#include "Global.h"




GLuint interior_tractor, interior_mercedes;
GLuint tractor, mercedes;

VEHICULOS vehiculo = MERCEDES;


extern float x;
extern float y;
extern float z;

extern CAMARAS camara;
extern bool elementos_solidarios_camara_activos;

void cargaTexturasVehiculos() {
	glGenTextures(1, &interior_tractor);
	glBindTexture(GL_TEXTURE_2D, interior_tractor);
	loadImageFile((char*)"interior_tractor.png");

	glGenTextures(1, &interior_mercedes);
	glBindTexture(GL_TEXTURE_2D, interior_mercedes);
	loadImageFile((char*)"interior_mercedes.png");

	glGenTextures(1, &tractor);
	glBindTexture(GL_TEXTURE_2D, tractor);
	loadImageFile((char*)"tractor.png");

	glGenTextures(1, &mercedes);
	glBindTexture(GL_TEXTURE_2D, mercedes);
	loadImageFile((char*)"mercedes.png");

}



void dibuja_interior_vehiculo() {
	if (camara != CAMARA_NORMAL) {
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glEnable(GL_BLEND); // habilito blending
		glEnable(GL_CULL_FACE); // quitar efecto raro
		//culling
		glCullFace(GL_BACK);
		//blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDepthMask(GL_TRUE);
		//dejar de generar texturas spheriacas generadas
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		// *_________________________________________________________________
		//objetos traslucidos que van pegados a la camara
		// quiero camara ortografica
		glPushMatrix(); // apila la modelView
		glLoadIdentity(); // limpio el look at
		glMatrixMode(GL_PROJECTION);
		glPushMatrix(); //apilar la proyeccion
		glLoadIdentity(); // vista por defecto
		glOrtho(-1, 1, -1, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
		if (camara == CAMARA_INTERIOR) {
			if (vehiculo == TRACTOR) {
				glBindTexture(GL_TEXTURE_2D, interior_tractor);
			}
			else if (vehiculo == MERCEDES) {
				glBindTexture(GL_TEXTURE_2D, interior_mercedes);
			}

		}
		else if (camara == CAMARA_VER_VEHICULO) {
			if (vehiculo == TRACTOR) {
				glBindTexture(GL_TEXTURE_2D, tractor);
			}
			else if (vehiculo == MERCEDES) {
				glBindTexture(GL_TEXTURE_2D, mercedes);
			}
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // luces no afectan
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 0);

		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 0);

		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 0);
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		// *_______________________________________________

		glPopAttrib();
	}
}

#endif