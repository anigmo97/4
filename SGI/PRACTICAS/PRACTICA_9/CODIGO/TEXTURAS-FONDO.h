#ifndef _TEXTURAS_FONDO_H_
#define _TEXTURAS_FONDO_H_

#include "Global.h"

#define ALTURA_CILINDRO_FONDO 300
#define MITAD_ALTURA_CILINDRO ALTURA_CILINDRO_FONDO/2
#define TERCIO_ALTURA_CILINDRO ALTURA_CILINDRO_FONDO/3
#define RADIO_CILINDRO_FONDO 150
#define ANGULO_INICIAL 270
#define NUM_QUADS_CILINDRO 10


GLuint textura_fondo;

GLfloat vq0[3] = { 0,-TERCIO_ALTURA_CILINDRO,0 };
GLfloat vq1[3] = { 0,-TERCIO_ALTURA_CILINDRO,0 };
GLfloat vq2[3] = { 0,2 * TERCIO_ALTURA_CILINDRO,0 };
GLfloat vq3[3] = { 0,2 * TERCIO_ALTURA_CILINDRO,0 };

/*CONSTANTES PRECALCULADAS*/
static const float DOS_PI_PARTIDO_DIEZ = 2 * PI / 10;

extern float x;
extern float y;
extern float z;

void cargaTexturaFondo() {
	glGenTextures(1, &textura_fondo);
	glBindTexture(GL_TEXTURE_2D, textura_fondo);
	loadImageFile((char*)"desierto.jpg");

}


void pasar_v1_a_v0_quad_cilindro(void) {
	vq0[X_DIM] = vq1[X_DIM];
	vq0[Z_DIM] = vq1[Z_DIM];
}

void pasar_v2_a_v3_quad_cilindro(void) {
	vq3[X_DIM] = vq2[X_DIM];
	vq3[Z_DIM] = vq2[Z_DIM];
}

void calcula_vq1_y_vq2(float angulo) {

	vq1[X_DIM] = x + (sin(angulo) * RADIO_CILINDRO_FONDO);
	vq1[Z_DIM] = z + (-cos(angulo) * RADIO_CILINDRO_FONDO);

	vq2[X_DIM] = x + (sin(angulo) * RADIO_CILINDRO_FONDO);
	vq2[Z_DIM] = z + (-cos(angulo) * RADIO_CILINDRO_FONDO);

}

void dibuja_cilindro_fondo() {
	glBindTexture(GL_TEXTURE_2D, textura_fondo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Texel menor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Texel mayor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // repetir texturas
	//mirror da la vuelta a la textura al pegarla
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // repetir texturas
	//glColor3f(0, 1, 0);
	float angulo;
	float s_min;
	float s_max;
	for (auto i = 0; i < (NUM_QUADS_CILINDRO + 1); i++) {
		angulo = ANGULO_INICIAL - i * DOS_PI_PARTIDO_DIEZ;
		calcula_vq1_y_vq2(angulo);
		if (i > 0) {
			s_min = (float)(i - 1) * 4 / (float)NUM_QUADS_CILINDRO;
			s_max = (float)(i) * 4 / (float)NUM_QUADS_CILINDRO;
			//cout << "( " << s_min << ", "<< s_max << ")\n";
			quadtex(vq0, vq1, vq2, vq3, s_min, s_max, 0, 1, 20, 20);
		}
		pasar_v1_a_v0_quad_cilindro();
		pasar_v2_a_v3_quad_cilindro();
	}

	//cin.get();
}
#endif