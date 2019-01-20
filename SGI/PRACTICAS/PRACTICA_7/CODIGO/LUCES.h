#ifndef _LUCES_H_
#define _LUCES_H_

#include "Global.h"
#include "CARRETERA.h"

/* VALORES POR DEFECTO*/
GLfloat material_color_ambiental_defecto[] = { 0.2, 0.2, 0.2 };
GLfloat material_color_emisivo_defecto[] = { 0, 0, 0 };

extern float x;

void inicializa_Luces(void) {

	/* LIGHT0 ILUMINACION DIRECCIONAL LUNA */
	GLfloat Al0[] = { 0.05,0.05,0.05,0.05 }; // Color ambiental de la luz
	GLfloat Dl0[] = { 0.05,0.05,0.05 }; // Color difuso de la luz
	GLfloat Sl0[] = { 0.0, 0.0, 0.0 }; // Color especular de la luz
	glLightfv(GL_LIGHT0, GL_AMBIENT, Al0); // Caracteristicas de LIGHT0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0);

	/* LIGHT1 FARO VEHICULO*/
	//GLfloat Al1[] = { 0.2, 0.2, 0.2 }; // Color ambiental de la luz
	GLfloat Al1[] = { 1.0, 1.0, 1.0 };
	GLfloat Dl1[] = { 1.0, 1.0, 1.0 }; // Color difuso de la luz
	GLfloat Sl1[] = { 0.3, 0.3, 0.3 }; // Color especular de la luz
	//GLfloat Sl1[] = { 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, Al1); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Dl1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Sl1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);

	/* LIGHT2 - LIGHT5 FAROLAS CARRETERA*/
	GLfloat Al2[] = { 1.0, 1.0, 1.0 }; // Color ambiental de la luz
	GLfloat Dl2[] = { 1.0, 1.0, 1.0 }; // Color difuso de la luz
	GLfloat Sl2[] = { 1.0, 1.0, 1.0 }; // Color especular de la luz

	glLightfv(GL_LIGHT2, GL_AMBIENT, Al2); 
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0); // angulo
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0); // atenuacion

	glLightfv(GL_LIGHT3, GL_AMBIENT, Al2); // Caracteristicas de LIGHT2
	glLightfv(GL_LIGHT3, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT3, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0); // angulo
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0); // atenuacion

	glLightfv(GL_LIGHT4, GL_AMBIENT, Al2); // Caracteristicas de LIGHT2
	glLightfv(GL_LIGHT4, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT4, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0); // angulo
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0); // atenuacion

	glLightfv(GL_LIGHT5, GL_AMBIENT, Al2); // Caracteristicas de LIGHT2
	glLightfv(GL_LIGHT5, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT5, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0); // angulo
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0); // atenuacion
}

void habilita_Luces(void) {
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
}

void setPosicion_y_direccion_faro(void) {
	// Para la luz de minero situamos la luz antes de situar la camara
	// asi no le afecta la VIEW y trabajamos en coordenadas del sistema de la camara
	GLfloat posicion[] = { 0, 0.7, 0, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, posicion); // Luz focal sobre la camara
	GLfloat dir_central[] = { 0, -0.5, -0.7 }; // Direccion del foco la de la vista
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_central);
}



void set_material_carretera(void) {
	GLfloat difuso[] = { 0.8,0.8, 0.8 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difuso);

	GLfloat especular[] = { 0.3,0.3,0.3 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, especular);

	//glMaterialf(GL_FRONT, GL_EMI, material_color_emisivo_defecto);
	glMaterialf(GL_FRONT, GL_SHININESS, 3);


}

void set_material_linea_carretera() {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, AMARILLO);//color
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLANCO); //brillo 
	glMaterialf(GL_FRONT, GL_SHININESS, 100);
}

void coloca_luces_farolas(int x_inicial_1) {
	while (x_inicial_1 % (int)separacion_farolas != 0) {
		x_inicial_1++;
	}
	int puntos_en_x[4];
	float puntos_en_z[4];
	//cout << "punto en x = " << punto_en_x<<"\n";
	for (auto i = 1; i < 5; i++) {
		puntos_en_x[i-1] =  (x_inicial_1+separacion_farolas*i);
		puntos_en_z[i-1] = funcion_f(puntos_en_x[i-1]);
	}
	/*for (auto i = 0; i < 4; i++) {
		cout << "x" << i << "=" << puntos_en_x[i] << "  z" << i << "=" << puntos_en_z[i] << "\n";
	}
	cout << "\n\n";*/

	//cin.get();

	GLfloat pos[] = { puntos_en_x[0], 4, puntos_en_z[0], 1 };
	GLfloat dir[] = { 0, -1, 0 }; // Direccion del foco la de la vista
	glLightfv(GL_LIGHT2, GL_POSITION, pos); // Luz focal sobre la camara
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir);

	pos[0] = puntos_en_x[1];
	pos[2] = puntos_en_z[1];
	glLightfv(GL_LIGHT3, GL_POSITION, pos); // Luz focal sobre la camara
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dir);

	pos[0] = puntos_en_x[2];
	pos[2] = puntos_en_z[2];
	glLightfv(GL_LIGHT4, GL_POSITION, pos); // Luz focal sobre la camara
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dir);

	pos[0] = puntos_en_x[3];
	pos[2] = puntos_en_z[3];
	glLightfv(GL_LIGHT5, GL_POSITION, pos); // Luz focal sobre la camara
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, dir);
}

#endif
