#ifndef _TEXTURAS_ELEMENTOS_SOL_H_
#define _TEXTURAS_ELEMENTOS_SOL_H_

#include "Global.h"

GLuint velocimetro, palo;
GLuint barra_coordenadas, flecha;


GLfloat v0_aguja[3] = { 0,0,0 };
GLfloat v1_aguja[3] = { 0,0,0 };
GLfloat v2_aguja[3] = { 0,0,0 };
GLfloat v3_aguja[3] = { 0,0,0 };


float origen_aguja[2] = { -0.6,-0.83 };
float vector_n_aguja[2];

extern float x;
extern float y;
extern float z;
extern CAMARAS camara;
extern bool elementos_solidarios_camara_activos;

void cargaTexturasElementosSolidarios() {
	glGenTextures(1, &velocimetro);
	glBindTexture(GL_TEXTURE_2D, velocimetro);
	loadImageFile((char*)"velocimetro.png");

	glGenTextures(1, &palo);
	glBindTexture(GL_TEXTURE_2D, palo);
	loadImageFile((char*)"palo.png");

	glGenTextures(1, &flecha);
	glBindTexture(GL_TEXTURE_2D, flecha);
	loadImageFile((char*)"flecha2.png");

	glGenTextures(1, &barra_coordenadas);
	glBindTexture(GL_TEXTURE_2D, barra_coordenadas);
	loadImageFile((char*)"barra-coords.png");
}



float calcula_angulo_dada_velocidad(float vel) {
	if (vel < 50) {
		float ang = -max(0, 76 - 1.52*vel);
		if (ang < 0) {
			return ang + 360;
		}
		else {
			return ang;
		}
	}
	else if (vel > 50) {
		return min(76, 1.52*vel - 76);
	}
	else {
		return 0;
	}
}

void calcula_vector_n_normalizado_aguja(float angulo) {
	float angulo_trabajo;
	if (angulo + 90 >= 360) {
		angulo_trabajo = angulo - 90;
	}
	else {
		angulo_trabajo = angulo + 90;
	}

	vector_n_aguja[X_DIM] = sin(angulo_trabajo* (PI / 180));
	vector_n_aguja[Y_DIM] = cos(angulo_trabajo* (PI / 180));
}

float calcula_coord_x_aguja_dado_angulo(float angulo) {
	float seno = sin(angulo* (PI / 180));
	return -0.6 + seno * 0.3;
}

float calcula_coord_y_aguja_dado_angulo(float angulo) {
	return -0.83 + cos(angulo* (PI / 180)) * 0.6;
}

void calcula_vectores_aguja(float coord_x, float coord_y) {
	v0_aguja[X_DIM] = origen_aguja[X_DIM] - vector_n_aguja[X_DIM] * 0.005;
	v0_aguja[Y_DIM] = origen_aguja[Y_DIM] - vector_n_aguja[Y_DIM] * 0.005;

	v1_aguja[X_DIM] = origen_aguja[X_DIM] + vector_n_aguja[X_DIM] * 0.005;
	v1_aguja[Y_DIM] = origen_aguja[Y_DIM] + vector_n_aguja[Y_DIM] * 0.005;

	v2_aguja[X_DIM] = coord_x + vector_n_aguja[X_DIM] * 0.005;
	v2_aguja[Y_DIM] = coord_y + vector_n_aguja[Y_DIM] * 0.005;

	v3_aguja[X_DIM] = coord_x - vector_n_aguja[X_DIM] * 0.005;
	v3_aguja[Y_DIM] = coord_y - vector_n_aguja[Y_DIM] * 0.005;
}

void crea_lista_dibujo_aguja() {

	float angulo_aguja = calcula_angulo_dada_velocidad(modulo_velocidad);
	float coord_x = calcula_coord_x_aguja_dado_angulo(angulo_aguja);
	float coord_y = calcula_coord_y_aguja_dado_angulo(angulo_aguja);
	calcula_vector_n_normalizado_aguja(angulo_aguja);
	calcula_vectores_aguja(coord_x, coord_y);


	/*cout << "v3 =( " << v3_aguja[X_DIM] << " , " << v3_aguja[Y_DIM]<<")   v2=( "<< v2_aguja[X_DIM] << " , " << v2_aguja[Y_DIM] << ")\n";
	cout << "v0 =( " << v0_aguja[X_DIM] << " , " << v0_aguja[Y_DIM] << ")   v1=( " << v1_aguja[X_DIM] << " , " << v1_aguja[Y_DIM] << ")\n\n\n\n";*/

	glPushMatrix();
	if (modulo_velocidad <= 50) {
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);

		glTexCoord2f(0, 0);
		glVertex3f(v1_aguja[X_DIM], v1_aguja[Y_DIM], -0.2);


		glTexCoord2f(1, 0);
		glVertex3f(v0_aguja[X_DIM], v0_aguja[Y_DIM], -0.2);

		glTexCoord2f(1, 1);
		glVertex3f(v3_aguja[X_DIM], v3_aguja[Y_DIM], -0.2);


		glTexCoord2f(0, 1);
		glVertex3f(v2_aguja[X_DIM], v2_aguja[Y_DIM], -0.2);
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);

		glTexCoord2f(0, 0);
		glVertex3f(v0_aguja[X_DIM], v0_aguja[Y_DIM], -0.2);

		glTexCoord2f(1, 0);
		glVertex3f(v1_aguja[X_DIM], v1_aguja[Y_DIM], -0.2);

		glTexCoord2f(1, 1);
		glVertex3f(v2_aguja[X_DIM], v2_aguja[Y_DIM], -0.2);

		glTexCoord2f(0, 1);
		glVertex3f(v3_aguja[X_DIM], v3_aguja[Y_DIM], -0.2);

		glEnd();
	}
	glPopMatrix();
}

void dibuja_aguja_velocimetro() {
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

	glBindTexture(GL_TEXTURE_2D, palo);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // luces no afectan
	crea_lista_dibujo_aguja(); // <------------------------------------------------------------
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// *_______________________________________________

	glPopAttrib();

}

void dibuja_velocimetro() {
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

	glBindTexture(GL_TEXTURE_2D, velocimetro);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // luces no afectan
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);

	glTexCoord2f(0, 0);
	glVertex3f(-1, -1, -0.2);

	glTexCoord2f(1, 0);
	glVertex3f(-0.2, -1, -0.2);

	glTexCoord2f(1, 1);
	glVertex3f(-0.2, 0, -0.2);

	glTexCoord2f(0, 1);
	glVertex3f(-1, 0, -0.2);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// *_______________________________________________

	glPopAttrib();
	dibuja_aguja_velocimetro();
}


void dibuja_barra_coordenadas() {
	float punto_barra = (angulo / 360) + 0.354;
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

	glBindTexture(GL_TEXTURE_2D, barra_coordenadas);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // luces no afectan
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);

	glTexCoord2f(punto_barra - 0.15, 0);
	glVertex3f(0.2, -0.8, -0.2);

	glTexCoord2f(punto_barra + 0.15, 0);
	glVertex3f(0.8, -0.8, -0.2);

	glTexCoord2f(punto_barra + 0.15, 1);
	glVertex3f(0.8, -0.6, -0.2);

	glTexCoord2f(punto_barra - 0.15, 1);
	glVertex3f(0.2, -0.6, -0.2);
	glEnd();




	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// *_______________________________________________

	glPopAttrib();
}

void dibuja_flecha() {

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

	glBindTexture(GL_TEXTURE_2D, flecha);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // luces no afectan

	glBegin(GL_POLYGON); //FLECHA
	glNormal3f(0, 0, 1);

	glTexCoord2f(0, 0);
	glVertex3f(0.45, -0.95, -0.2);

	glTexCoord2f(1, 0);
	glVertex3f(0.55, -0.95, -0.2);

	glTexCoord2f(1, 1);
	glVertex3f(0.55, -0.83, -0.2);

	glTexCoord2f(0, 1);
	glVertex3f(0.45, -0.83, -0.2);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// *_______________________________________________

	glPopAttrib();
}

void dibuja_elementos_solidarios_camara() {
	if (elementos_solidarios_camara_activos) {
		camara = CAMARA_NORMAL;
		dibuja_velocimetro();
		dibuja_barra_coordenadas();
		dibuja_flecha();
	}
}


#endif
