#ifndef _TEXTURAS_CARTELES_H_
#define _TEXTURAS_CARTELES_H_

#include "Global.h"




GLuint texturas_carteles[4];
GLuint indices_texturas_carteles[4] = { 0,1,2,3 };


GLfloat v0_cartel[3] = { 0,0,0 };
GLfloat v1_cartel[3] = { 0,0,0 };
GLfloat v2_cartel[3] = { 0,0,0 };
GLfloat v3_cartel[3] = { 0,0,0 };


GLfloat coordenadas_x_palo_1[4] = { 0,0,0,0 };
GLfloat coordenadas_z_palo_1[4] = { 0,0,0,0 };
GLfloat coordenadas_x_palo_2[4] = { 0, 0, 0, 0 };
GLfloat coordenadas_z_palo_2[4] = { 0, 0, 0, 0 };

//TODO inicializar
GLfloat coordenadas_x_luces[4];
GLfloat coordenadas_z_luces[4];

/*VECTORES CALCULO palos carteles*/
float vector_t_palo[2];
float vector_n_palo[2];

extern float x;
extern float y;
extern float z;


void cargaTexturasCarteles() {


	glGenTextures(1, &texturas_carteles[0]);
	glBindTexture(GL_TEXTURE_2D, texturas_carteles[0]);
	loadImageFile((char*)"cartel1.jpg");

	glGenTextures(1, &texturas_carteles[1]);
	glBindTexture(GL_TEXTURE_2D, texturas_carteles[1]);
	loadImageFile((char*)"cartel2.jpeg");

	glGenTextures(1, &texturas_carteles[2]);
	glBindTexture(GL_TEXTURE_2D, texturas_carteles[2]);
	loadImageFile((char*)"cartel3.png");

	glGenTextures(1, &texturas_carteles[3]);
	glBindTexture(GL_TEXTURE_2D, texturas_carteles[3]);
	loadImageFile((char*)"cartel4.jpg");


}



void dibuja_palo(GLfloat pos_x, GLfloat pos_z) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glTranslatef(pos_x, 0, pos_z);
	glTranslatef(0, 7, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.3, 8, 10, 10);
	glPopMatrix();
	glPopAttrib();
}



void actualiza_posicion_heredada_luces() {
	coordenadas_x_luces[0] = puntos_en_x_luces[0];
	coordenadas_x_luces[1] = puntos_en_x_luces[1];
	coordenadas_x_luces[2] = puntos_en_x_luces[2];
	coordenadas_x_luces[3] = puntos_en_x_luces[3];

	coordenadas_z_luces[0] = puntos_en_z_luces[0];
	coordenadas_z_luces[1] = puntos_en_z_luces[1];
	coordenadas_z_luces[2] = puntos_en_z_luces[2];
	coordenadas_z_luces[3] = puntos_en_z_luces[3];
}

void calcula_vectores_t_y_n_normalizados_palos(float punto_en_x) {
	float f_derivada = derivada_funcion_f(punto_en_x);
	float aux = 1 / sqrt(1 + f_derivada * f_derivada);
	float aux2 = aux * f_derivada;
	vector_t_palo[0] = aux;
	vector_t_palo[1] = aux2;
	vector_n_palo[0] = -aux2;
	vector_n_palo[1] = aux;
}

float calcula_dim_vector_para_punto_palo(float punto, PALOS_CARTEL vec, DIMENSION dim) {
	switch (vec) {
	case PALO_IZQUIERDA:
		if (dim == X_DIM) {
			return punto - vector_n_palo[0] * WIDTH_QUAD / 2;
		}
		else if (dim == Z_DIM) {
			return punto - vector_n_palo[1] * WIDTH_QUAD / 2;
		}
		break;
	case PALO_DERECHA:
		if (dim == X_DIM) {
			return punto + vector_n_palo[0] * WIDTH_QUAD / 2;
		}
		else if (dim == Z_DIM) {
			return punto + vector_n_palo[1] * WIDTH_QUAD / 2;
		}
		break;
	default:
		break;
	}
}

void actualiza_coordenada_palo(float pos_x, float pos_z, int num_palo) {
	calcula_vectores_t_y_n_normalizados_palos(pos_x);

	coordenadas_x_palo_1[num_palo] = calcula_dim_vector_para_punto_palo(pos_x, PALO_IZQUIERDA, X_DIM);
	coordenadas_z_palo_1[num_palo] = calcula_dim_vector_para_punto_palo(pos_z, PALO_IZQUIERDA, Z_DIM);

	coordenadas_x_palo_2[num_palo] = calcula_dim_vector_para_punto_palo(pos_x, PALO_DERECHA, X_DIM);
	coordenadas_z_palo_2[num_palo] = calcula_dim_vector_para_punto_palo(pos_z, PALO_DERECHA, Z_DIM);
}

void actualiza_coordenadas_carteles() {
	if (coordenadas_x_luces[0] != puntos_en_x_luces[0]) {
		actualiza_posicion_heredada_luces();
		for (auto i = 0; i < 4; i++) {
			actualiza_coordenada_palo(coordenadas_x_luces[i], coordenadas_z_luces[i], i);
		}
		int aux = indices_texturas_carteles[0];
		indices_texturas_carteles[0] = indices_texturas_carteles[1];
		indices_texturas_carteles[1] = indices_texturas_carteles[2];
		indices_texturas_carteles[2] = indices_texturas_carteles[3];
		indices_texturas_carteles[3] = aux;
	}
}

void actualiza_puntos_quad_cartel(int num_cartel) {
	v0_cartel[X_DIM] = coordenadas_x_palo_1[num_cartel];
	v0_cartel[Y_DIM] = 4.1;
	v0_cartel[Z_DIM] = coordenadas_z_palo_1[num_cartel];

	v1_cartel[X_DIM] = coordenadas_x_palo_2[num_cartel];
	v1_cartel[Y_DIM] = 4.1;
	v1_cartel[Z_DIM] = coordenadas_z_palo_2[num_cartel];

	v2_cartel[X_DIM] = coordenadas_x_palo_2[num_cartel];
	v2_cartel[Y_DIM] = 7;
	v2_cartel[Z_DIM] = coordenadas_z_palo_2[num_cartel];

	v3_cartel[X_DIM] = coordenadas_x_palo_1[num_cartel];
	v3_cartel[Y_DIM] = 7;
	v3_cartel[Z_DIM] = coordenadas_z_palo_1[num_cartel];

}

void fija_textura_cartel(int num_cartel) {
	glBindTexture(GL_TEXTURE_2D, texturas_carteles[indices_texturas_carteles[num_cartel]]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Texel menor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Texel mayor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // repetir texturas
	//mirror da la vuelta a la textura al pegarla
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // repetir texturas
}

void dibuja_carteles() {
	actualiza_coordenadas_carteles();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor3f(0.7, 0.7, 0.7);
	for (int i = 0; i < 4; i++) {
		dibuja_palo(coordenadas_x_palo_1[i], coordenadas_z_palo_1[i]);
		dibuja_palo(coordenadas_x_palo_2[i], coordenadas_z_palo_2[i]);
		actualiza_puntos_quad_cartel(i);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		fija_textura_cartel(i);
		quadtex(v0_cartel, v1_cartel, v2_cartel, v3_cartel, 0, 1, 0, 1, 20, 20);
		glPopAttrib();
	}
	glPopAttrib();
}
#endif