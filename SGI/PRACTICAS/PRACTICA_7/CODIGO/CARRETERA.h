#ifndef _CARRETERA_H_
#define _CARRETERA_H_

#include "Global.h"

#define LENGHT_QUAD 2
#define WIDTH_QUAD 6
#define NUM_TRAMOS 50
#define WIDTH_RAYA_CENTRAL_CARRETERA 0.2

typedef enum {
	QUAD_ARRIBA = 0,
	RAYA_CENTRAL_AMARILLA = 1,
	QUAD_ABAJO = 2,
	QUAD_COMPLETO = 3
} PARTE_CARRETERA;

/*VARIABLES GENERCIÓN CARRETERA*/
static const float amplitud_A = 10;
static const float periodo_T = 50;
static const float separacion_farolas = amplitud_A*1.5 ;
/*CONSTANTES PRECALCULADAS*/
static const float DOS_PI_PARTIDO_PERIODO = 2 * PI / periodo_T;
static const float DOS_PI_A_PARTIDO_PERIODO = (2 * PI * amplitud_A) / periodo_T;
/*VECTORES CALCULO CARRETERA*/
float vector_t[2];
float vector_n[2];
/*CALCULO QUAD*/
float punto_en_x;
float punto_en_z;

/*VERTICES EXTERNOS QUAD*/
extern GLfloat v0[3];
extern GLfloat v1[3];
extern GLfloat v2[3];
extern GLfloat v3[3];
/*VERTICES INTERNOS QUAD (LINEA AMARILLA) */
extern GLfloat v0_interior[3];
extern GLfloat v1_interior[3];
extern GLfloat v2_interior[3];
extern GLfloat v3_interior[3];

float calcula_modulo_vector(float  vector[3]) {
	float suma = vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
	return sqrt(suma);
}

float funcion_f(float punto_en_x) {
	//CAMBIAMOS EL SIGNO A LO QUE DEVUELVE LA FUNCION PORQUE ESTAMOS EN EL EJE XZ Y ES AL CONTRARIO QUE Y
	return +(amplitud_A * sin(punto_en_x * DOS_PI_PARTIDO_PERIODO));
}

float derivada_funcion_f(float punto_en_x) {
	return DOS_PI_A_PARTIDO_PERIODO * cos(punto_en_x* (DOS_PI_PARTIDO_PERIODO));
}

void calcula_vectores_t_y_n_normalizados() {
	float f_derivada = derivada_funcion_f(punto_en_x);
	float aux = 1 / sqrt(1 + f_derivada * f_derivada);
	float aux2 = aux * f_derivada;
	vector_t[0] = aux;
	vector_t[1] = aux2;
	vector_n[0] = -aux2;
	vector_n[1] = aux;
}

void calcula_punto_en_x_y_punto_en_z_iniciales(float x) {
	punto_en_x = ((int)x) - 10;
	punto_en_z = funcion_f(punto_en_x);
}

void actualiza_punto_en_x_y_z() {
	punto_en_x = punto_en_x + vector_t[0] * LENGHT_QUAD;
	punto_en_z = funcion_f(punto_en_x); // poner - ???
}

float calcula_dim_vector_para_punto(float punto, VECTORES_QUAD vec, DIMENSION dim) {
	switch (vec) {
	case VECTOR_0:
		if (dim == X_DIM) {
			return punto - vector_n[0] * WIDTH_QUAD / 2;
		}
		else if (dim == Z_DIM) {
			return punto - vector_n[1] * WIDTH_QUAD / 2;
		}
		break;
	case VECTOR_1:
		if (dim == X_DIM) {
			return punto - vector_n[0] * WIDTH_QUAD / 2;
		}
		else if (dim == Z_DIM) {
			return punto - vector_n[1] * WIDTH_QUAD / 2;
		}
		break;
	case VECTOR_2:
		if (dim == X_DIM) {
			return punto + vector_n[0] * WIDTH_QUAD / 2;
		}
		else if (dim == Z_DIM) {
			return punto + vector_n[1] * WIDTH_QUAD / 2;
		}
		break;
	case VECTOR_3:
		if (dim == X_DIM) {
			return punto + vector_n[0] * WIDTH_QUAD / 2;
		}
		else if (dim == Z_DIM) {
			return punto + vector_n[1] * WIDTH_QUAD / 2;
		}
		break;
	default:
		break;
	}
}

float calcula_dim_vector_para_punto_interior(float punto, VECTORES_QUAD vec, DIMENSION dim) {
	switch (vec) {
	case VECTOR_0:
		if (dim == X_DIM) {
			return punto - vector_n[0] * WIDTH_RAYA_CENTRAL_CARRETERA / 2;
		}
		else if (dim == Z_DIM) {
			return punto - vector_n[1] * WIDTH_RAYA_CENTRAL_CARRETERA / 2;
		}
		break;
	case VECTOR_1:
		if (dim == X_DIM) {
			return punto - vector_n[0] * WIDTH_RAYA_CENTRAL_CARRETERA / 2;
		}
		else if (dim == Z_DIM) {
			return punto - vector_n[1] * WIDTH_RAYA_CENTRAL_CARRETERA / 2;
		}
		break;
	case VECTOR_2:
		if (dim == X_DIM) {
			return punto + vector_n[0] * WIDTH_RAYA_CENTRAL_CARRETERA / 2;
		}
		else if (dim == Z_DIM) {
			return punto + vector_n[1] * WIDTH_RAYA_CENTRAL_CARRETERA / 2;
		}
		break;
	case VECTOR_3:
		if (dim == X_DIM) {
			return punto + vector_n[0] * WIDTH_RAYA_CENTRAL_CARRETERA / 2;
		}
		else if (dim == Z_DIM) {
			return punto + vector_n[1] * WIDTH_RAYA_CENTRAL_CARRETERA / 2;
		}
		break;
	default:
		break;
	}
}

void calcula_vertice(VECTORES_QUAD vec) {
	switch (vec) {
	case VECTOR_0: break;
	case VECTOR_1:
		v1[X_DIM] = calcula_dim_vector_para_punto(punto_en_x, VECTOR_1, X_DIM);
		v1[Z_DIM] = calcula_dim_vector_para_punto(punto_en_z, VECTOR_1, Z_DIM);
		break;
	case VECTOR_2:
		v2[X_DIM] = calcula_dim_vector_para_punto(punto_en_x, VECTOR_2, X_DIM);
		v2[Z_DIM] = calcula_dim_vector_para_punto(punto_en_z, VECTOR_2, Z_DIM);
		break;
	case VECTOR_3: break;
	default:
		break;
	}
}


void calcula_vertice_interior(VECTORES_QUAD vec) {
	switch (vec) {
	case VECTOR_0: break;
	case VECTOR_1:
		v1_interior[X_DIM] = calcula_dim_vector_para_punto_interior(punto_en_x, VECTOR_1, X_DIM);
		v1_interior[Z_DIM] = calcula_dim_vector_para_punto_interior(punto_en_z, VECTOR_1, Z_DIM);
		break;
	case VECTOR_2:
		v2_interior[X_DIM] = calcula_dim_vector_para_punto_interior(punto_en_x, VECTOR_2, X_DIM);
		v2_interior[Z_DIM] = calcula_dim_vector_para_punto_interior(punto_en_z, VECTOR_2, Z_DIM);
		break;
	case VECTOR_3: break;
	default:
		break;
	}
}

void pasar_v1_a_v0(void) {
	v0[X_DIM] = v1[X_DIM];
	v0[Z_DIM] = v1[Z_DIM];

	v0_interior[X_DIM] = v1_interior[X_DIM];
	v0_interior[Z_DIM] = v1_interior[Z_DIM];
}

void pasar_v2_a_v3(void) {
	v3[X_DIM] = v2[X_DIM];
	v3[Z_DIM] = v2[Z_DIM];

	v3_interior[X_DIM] = v2_interior[X_DIM];
	v3_interior[Z_DIM] = v2_interior[Z_DIM];
}

void dibuja_carretera(PARTE_CARRETERA p) {
	switch (p)
	{
	case QUAD_ARRIBA:
		quad(v3_interior, v2_interior, v2, v3, 20, 20);
		break;
	case QUAD_ABAJO:
		quad(v0, v1, v1_interior, v0_interior, 20, 20);
		break;
	case RAYA_CENTRAL_AMARILLA:
		glColor3f(0.5, 0.5, 0);
		quad(v0_interior, v1_interior, v2_interior, v3_interior, 20, 20);
		glColor3f(0, 0, 0);
		break;
	case QUAD_COMPLETO:
		glPushAttrib(GL_COLOR);
			glColor3f(0, 0, 0);
			//quad(v0, v1, v2, v3, 20, 20);
			quadtex(v0, v1, v2, v3, 0, 1, 0, 1, 20, 20);
		glPopAttrib();
	default:
		break;
	}
}

#endif
