
#ifndef _iostream_
#define _iostream_
#include <iostream>
#endif

#ifndef _gl_freeglut_h_
#define _gl_freeglut_h_
#include <gl/freeglut.h>
#endif

#ifndef _utilidades_h_
#define _utilidades_h_
#include <Utilidades.h>
#endif

#ifndef _sstream_
#define _sstream_
#include <sstream>
#endif

#ifndef _global_h_
#define _global_h_

typedef enum {
	X_DIM = 0,
	Y_DIM = 1,
	Z_DIM = 2
} DIMENSION;

typedef enum {
	VECTOR_0,
	VECTOR_1,
	VECTOR_2,
	VECTOR_3
} VECTORES_QUAD;

typedef enum {
	DIA,
	NOCHE,
}ESTADO_DIA;

typedef enum {
	SOLIDO,
	ALAMBRICO,
}RENDERIZADO;

GLfloat v0[3] = { 0,0,0 };
GLfloat v1[3] = { 0,0,0 };
GLfloat v2[3] = { 0,0,0 };
GLfloat v3[3] = { 0,0,0 };
GLfloat v0_interior[3] = { 0,0,0 };
GLfloat v1_interior[3] = { 0,0,0 };
GLfloat v2_interior[3] = { 0,0,0 };
GLfloat v3_interior[3] = { 0,0,0 };

typedef enum {
	PALO_IZQUIERDA,
	PALO_DERECHA
} PALOS_CARTEL;

typedef enum {
	CAMARA_NORMAL,
	CAMARA_INTERIOR,
	CAMARA_VER_VEHICULO
} CAMARAS;

typedef enum {
	TRACTOR,
	MERCEDES
} VEHICULOS;

#endif
