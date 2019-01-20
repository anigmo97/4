#ifndef _TEXTURA_PAUSA_H_
#define _TEXTURA_PAUSA_H_

#include "Global.h"

//#define RADIO_CILINDRO_FONDO 150

GLuint textura_pausa;
int contador_simbolo_pausa = 0;
extern float x;
extern float y;
extern float z;

void cargaTexturaPausa() {
	//// Array con las texturas
	glGenTextures(1, &textura_pausa);
	glBindTexture(GL_TEXTURE_2D, textura_pausa);
	loadImageFile((char*)"pausa.png");

}

void dibuja_simbolo_pausa() {
	if ((int)(contador_simbolo_pausa / 50) % 2 == 0) {
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

		glBindTexture(GL_TEXTURE_2D, textura_pausa);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // luces no afectan
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);

		glTexCoord2f(0, 0);
		glVertex3f(-0.9, 0.5, -0.2);

		glTexCoord2f(1, 0);
		glVertex3f(-0.6, 0.5, -0.2);

		glTexCoord2f(1, 1);
		glVertex3f(-0.6, 0.9, -0.2);

		glTexCoord2f(0, 1);
		glVertex3f(-0.9, 0.9, -0.2);
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		// *_______________________________________________

		glPopAttrib();
	}
	contador_simbolo_pausa++;
}

#endif