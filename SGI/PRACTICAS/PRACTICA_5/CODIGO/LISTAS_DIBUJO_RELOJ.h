#pragma once
/**
*	PRACTICA5.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut
*
**/

#include <iostream>;
#include <gl/freeglut.h>;
#include <Utilidades.h>
#include <ctime>



GLint marcas_grandes_reloj; // int en openGL (ajusta su tamaño en bits y int no)
GLint marcas_pequeñas_reloj;
GLint circulo;
GLint aguja;
GLint segundero;
double angulo;

/* este metodo crea una lista de dibujo con las instrucciones para dibujar las marcas de las horas de reloj*/
void crea_lista_dibujo_marcas_grandes_reloj() {
	marcas_grandes_reloj = glGenLists(1); // le pasamos el numero de listas contiguas que queremos y nos retorna un int
	/* A partir del nombre de la lista de dibujo y el modo de compilacion creamos la lista de dibujo*/
	glNewList(marcas_grandes_reloj, GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT); // Encapsulamos o apilamos estado actual diciendole que queremos que guarde
			glColor3f(0, 0, 0); // fijamos el color en el que dibujaremos
			//iniciamos un triangle_strip
			for (int i = 0; i < 12; i++) {
				angulo = i * (2 * PI / 12);
				glBegin(GL_TRIANGLE_STRIP);
				/* AÑADIMOS VERTICES AL TRIANGLE_STRIP*/
					glVertex3f(0.9 * cos(angulo + 0.01), 0.9 * sin(angulo + 0.01), 0.0);
					glVertex3f(0.9 * cos(angulo - 0.01), 0.9 * sin(angulo - 0.01), 0.0);
					glVertex3f(0.85 * cos(angulo + 0.01), 0.85 * sin(angulo + 0.01), 0.0);
					glVertex3f(0.85 * cos(angulo - 0.01), 0.85 * sin(angulo - 0.01), 0.0);
				glEnd(); //Terminamos Triangle_Strip
			}

		glPopAttrib(); // desapilamos estado 
	glEndList(); // terminamos lista de dibujo
}

/* este metodo crea una lista de dibujo con las instrucciones para dibujar las marcas de las horas de reloj*/
void crea_lista_dibujo_marcas_pequeñas_reloj() {
	marcas_pequeñas_reloj = glGenLists(1); // le pasamos el numero de listas contiguas que queremos y nos retorna un int
	/* A partir del nombre de la lista de dibujo y el modo de compilacion creamos la lista de dibujo*/
	glNewList(marcas_pequeñas_reloj, GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT); // Encapsulamos o apilamos estado actual diciendole que queremos que guarde
			glColor3f(0.2, 0.2, 0.2); // fijamos el color en el que dibujaremos
	
			//iniciamos un triangle_strip
			for (int i = 0; i < 60; i++) {
				if (i % 5 != 0) {
					angulo = i * (2 * PI / 60);
					glBegin(GL_TRIANGLE_STRIP);
						/* AÑADIMOS VERTICES AL TRIANGLE_STRIP*/
						glVertex3f(0.9 * cos(angulo + 0.005), 0.9 * sin(angulo + 0.005), 0.0);
						glVertex3f(0.9 * cos(angulo - 0.005), 0.9 * sin(angulo - 0.005), 0.0);
						glVertex3f(0.88 * cos(angulo + 0.005), 0.88 * sin(angulo + 0.005), 0.0);
						glVertex3f(0.88 * cos(angulo - 0.005), 0.88 * sin(angulo - 0.005), 0.0);
					glEnd(); //Terminamos Triangle_Strip
				}
			}

		glPopAttrib(); // desapilamos estado 
	glEndList(); // terminamos lista de dibujo
}

/* CREA LISTA DE DIBUJO PARA HACER UN CIRCULO */
void crea_lista_dibujo_circulo() {
	circulo = glGenLists(1); // le pasamos el numero de listas contiguas que queremos y nos retorna un int
	/* A partir del nombre de la lista de dibujo y el modo de compilacion creamos la lista de dibujo*/
	glNewList(circulo, GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT); // Encapsulamos o apilamos estado actual diciendole que queremos que guarde
			//glColor3f(1, 0.0, 0.0); // fijamos el color en el que dibujaremos
			glBegin(GL_POLYGON);//como ira un poligono dentro de mi lista:
	
			for (int i = 0; i < 360; i++) {
				angulo = i * (2 * PI / 360);
				/* AÑADIMOS VERTICES AL POLIGONO */
				glVertex3f(1.0 * cos(angulo), 1.0 * sin(angulo), 0.0);
			}
			glEnd();
		glPopAttrib(); // desapilamos estado 
	glEndList();// terminamos lista de dibujo	

}

/* CREA LISTA DE DIBUJO PARA HACER UNA AGUJA */
void crea_lista_dibujo_aguja() {
	aguja = glGenLists(1); // le pasamos el numero de listas contiguas que queremos y nos retorna un int
	/* A partir del nombre de la lista de dibujo y el modo de compilacion creamos la lista de dibujo*/
	glNewList(aguja, GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT); // Encapsulamos o apilamos estado actual diciendole que queremos que guarde
				//glColor3f(0, 0, 0); // fijamos el color en el que dibujaremos
			glBegin(GL_POLYGON);//como ira un poligono dentro de mi lista:			
				//VERTICES DEL CUADRADO
				glVertex3f(0.03, 0, 0);
				glVertex3f(0.03, 0.72, 0);
				glVertex3f(-0.03, 0.72, 0);
				glVertex3f(-0.03, 0, 0);
			glEnd();

			glBegin(GL_POLYGON);
				//VERTICES DEL TRIANGULO
				glVertex3f(-0.1, 0.72, 0);
				glVertex3f(0, 0.84, 0);
				glVertex3f(0.1, 0.72, 0);
			glEnd();
		glPopAttrib(); // desapilamos estado 
	glEndList();// terminamos lista de dibujo
}

/* CREA LISTA DE DIBUJO PARA HACER SEGUNDERO */
void crea_lista_dibujo_segundero() {
	segundero = glGenLists(1); // le pasamos el numero de listas contiguas que queremos y nos retorna un int
	/* A partir del nombre de la lista de dibujo y el modo de compilacion creamos la lista de dibujo*/
	glNewList(segundero, GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT); // Encapsulamos o apilamos estado actual diciendole que queremos que guarde
			glColor3f(0, 0, 1); // fijamos el color en el que dibujaremos
			glBegin(GL_POLYGON);//como ira un poligono dentro de mi lista:			
				glVertex3f(0.01, 0, 0);
				glVertex3f(0.01, 0.87, 0);
				glVertex3f(-0.01, 0.87, 0);
				glVertex3f(-0.01, 0, 0);
			glEnd();
		glPopAttrib(); // desapilamos estado 
	glEndList();// terminamos lista de dibujo
}
