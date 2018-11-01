/**
*	PRACTICA4.cpp
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Oct,2018
*	@require: freeglut
*
**/

#define PROYECTO "PRACTICA_4 ESFERA CON ESTRELLAS DE DAVID" // Lo usamos para mostrar un mensaje

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h> //podemos cambiar freeglut por Utilidades.h (utilidades incluye freeglut y mas)


GLint estrella; // int en openGL (ajusta su tamaño en bits y int no)
float x;
float y;
float z;



/* este metodo crea una lista de dibujo con las instrucciones para dibujar la estrella de david*/
void crea_lista_dibujo_estrella() {
	estrella = glGenLists(1); // le pasamos el numero de listas contiguas que queremos y nos retorna un int
	/* A partir del nombre de la lista de dibujo y el modo de compilacion creamos la lista de dibujo*/
	glNewList(estrella, GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT); // Encapsulamos o apilamos estado actual diciendole que queremos que guarde
			//glColor3f(0.0, 0.0, 0.3); // fijamos el color en el que dibujaremos

			double angulo;

			/*CREA TRIANGULO HACIA ARRIBA */
			glBegin(GL_TRIANGLE_STRIP); //iniciamos un triangle_strip
				for (int i = 0; i < 4; i++) {
					angulo = i * (2 * PI / 3) + (PI / 2);
					/* AÑADIMOS VERTICES AL TRIANGLE_STRIP*/
					glVertex3f(1.0 * cos(angulo), 1.0 * sin(angulo), 0.0);
					glVertex3f(0.7 * cos(angulo), 0.7 * sin(angulo), 0.0);
				}
			glEnd(); //Terminamos Triangle_Strip

			/*CREA TRIANGULO HACIA ABAJO*/
			glBegin(GL_TRIANGLE_STRIP);
				for (int i = 0; i < 4; i++) {
					angulo = i * (2 * PI / 3) + (3 * PI / 2);
					glVertex3f(1.0 * cos(angulo), 1.0 * sin(angulo), 0.0);
					glVertex3f(0.7 * cos(angulo), 0.7 * sin(angulo), 0.0);
				}
			glEnd();

		glPopAttrib(); // desapilamos estado 
	glEndList(); // terminamos lista de dibujo
}



/* este metodo solo se ejecuta una vez, los calculos y las creaciones
	   de poligonos deberían hacerse aqui en lugar de en display() */
void init() {

	glClearColor(1, 1, 1, 1);  //fijar el color de borrado (fondo)
	glColor3f(1, 0, 0); //fijar el color de dibujo
	crea_lista_dibujo_estrella();
	glEnable(GL_DEPTH_TEST);//configurar motor de render
	// Desde aqui doy valores iniciales de la cámara. ( elegidos aleatoriamente )
	x = 1;
	y = 2;
	z = 3;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar el fondo y profundidad
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila

	// Situar y orientar la camara: punto de vista, punto de interes, vertical
	gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);

	//ejes(); // instruccion auxiliar para ver el eje x , y , z 
	GLfloat componentes_RGB[3] = { 0, 0, 0 };
	for (auto i = 0; i < 6; i++) {
		glPushMatrix();
			componentes_RGB[i % 3] += 0.4;
			glRotatef((360 / 12)*i,0,1,0);
			glCallList(estrella); // llamamos a la lista de dibujo y se ejecutan sus acciones
			glColor3f(componentes_RGB[0], componentes_RGB[1], componentes_RGB[2]);
		glPopMatrix();
	}
	glColor3f(0, 0, 1);
	glutWireSphere(1, 20, 20);
	glFlush();    // indicar que han acabado las ordenes de dibujo
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h); //utiliza toda el area de dibujo
	float razon_aspecto = float(w) / float(h);
	float apertura;
	float distancia;

	// Matriz de la proyeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* HACER QUE LA ESFERA SE AJUSTE AL TAMAÑO DE LA VENTANA (TOQUE ARRIBA Y ABAJO)  */
	distancia = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	apertura = asin(1 / distancia) * 2;
	apertura = (180 / PI) * apertura;
	gluPerspective(apertura, razon_aspecto, 0.1, 20); //defino una camara de perspectiva
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv); // Reserva Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // un buffer , buffer color y buffer profundidad
	glutInitWindowPosition(100, 200); //Posicionar la ventana de dibujo en la pantalla
	glutInitWindowSize(400, 400); // Ajustar tamanyo ventana
	glutCreateWindow(PROYECTO); //Crear la ventana

	/*Registro de las callbacks (funcion que se dispara cuando sucede algo)*/
	glutDisplayFunc(display); // Evento display -> Cuando hay que dibujar
	glutReshapeFunc(reshape); // Evento resize/reshape (cambio ventana (dimension etc)) NO NECESARIO

	init(); // el init hace los calculos y los ajustes antes de dibujar
	std::cout << PROYECTO  " en marcha" << std::endl; // print por la salida estandar
	glutMainLoop(); //Poner en marcha el bucle de atencion a eventos
}