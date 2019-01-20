#ifndef _TEXTURAS_SUELO_H_
#define _TEXTURAS_SUELO_H_

#include "Global.h"

//#define RADIO_CILINDRO_FONDO 150

GLuint textura_carretera;
GLuint textura_suelo;

extern float x;
extern float y;
extern float z;

void cargaTexturasSuelo_y_Carretera() {
	//// Array con las texturas
	glGenTextures(1, &textura_carretera);
	glBindTexture(GL_TEXTURE_2D, textura_carretera);
	loadImageFile((char*)"camino7.jpg");

	glGenTextures(1, &textura_suelo);
	glBindTexture(GL_TEXTURE_2D, textura_suelo);
	loadImageFile((char*)"cesped.jpg");
}

void fijarTexturaCarretera() {
	glBindTexture(GL_TEXTURE_2D, textura_carretera);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
}

void fija_texturas_suelo() {
	glBindTexture(GL_TEXTURE_2D, textura_suelo);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Texel menor que pixel
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Texel mayor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // repetir texturas
	//mirror da la vuelta a la textura al pegarla
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // repetir texturas
}

void set_material_suelo(void) {
	GLfloat difuso[] = { 0.8,0.8, 0.8 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difuso);

	GLfloat especular[] = { 0.3,0.3,0.3 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);

	//glMaterialf(GL_FRONT, GL_EMI, material_color_emisivo_defecto);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 3);

}

void dibuja_suelo() {
	set_material_suelo();
	fija_texturas_suelo();
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLfloat esquina_suelo_abajo_der[3] = { x - RADIO_CILINDRO_FONDO,-0.01,z - RADIO_CILINDRO_FONDO };
	GLfloat esquina_suelo_arriba_der[3] = { x + RADIO_CILINDRO_FONDO,-0.01,z - RADIO_CILINDRO_FONDO };
	GLfloat esquina_suelo_arriba_izq[3] = { x + RADIO_CILINDRO_FONDO,-0.01,z + RADIO_CILINDRO_FONDO };
	GLfloat esquina_suelo_abajo_izq[3] = { x - RADIO_CILINDRO_FONDO,-0.01,z + RADIO_CILINDRO_FONDO };
	quadtex(esquina_suelo_abajo_izq, esquina_suelo_abajo_der, esquina_suelo_arriba_der, esquina_suelo_arriba_izq, 0, 100, 0, 100, 20, 20);
	//glPopAttrib();

}

#endif