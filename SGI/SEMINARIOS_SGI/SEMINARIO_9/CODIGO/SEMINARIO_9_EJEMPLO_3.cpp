/**
*	EFECTOS.cpp
*
*   USO DE BLENDING NIEBLA
*
*	@author: anigmo@inf.upv.es
*	@version: v1.0 Dic,2018
*	@require: freeglut, UTILIDADES
*
**/

#define PROYECTO "SEMINARIO_9_EJEMPLO_1: TEXTURA_REFLECTANTE"

#include <iostream>
#include <gl/freeglut.h>
#include <Utilidades.h>

// Variables globales
static enum { ALAMBRICO, SOLIDO, DOBLE } modoDibujo;
static float giroxTR = 0, giroyTR = 0, giroxTV = 0, giroyTV = 0;
static int xanterior, yanterior;
static bool dragging = false; // variable control 

static GLubyte esRoja, esVerde;

GLuint tex0, tex1, tex2;

void onMenu(int valor) {
	//Callbak de atencion al menu de contxto
	if (valor == 0) {
		modoDibujo = ALAMBRICO;
	}
	else if (valor == 1) {
		modoDibujo = SOLIDO;
	}
	else {
		modoDibujo = DOBLE;
	}

	glutPostRedisplay();
}

void init() { // solo se ejecuta una vez
	glClearColor(1, 1, 1, 1.0); //fijar el color de borrado (fondo)
	glEnable(GL_DEPTH_TEST);//configurar motor de render
	glEnable(GL_LIGHTING); // CONFIGURAR CALCULO LUCES con esto saldran practicamente negras

	//CAMBIAR MODO SOMBREADO
	//glShadeModel(GL_FLAT); // quitar modelo gaudo
	glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D); //habilito las texturas

	glEnable(GL_BLEND); // habilito blending
	glEnable(GL_CULL_FACE); // quitar efecto raro

	glEnable(GL_FOG);

	// niebla
	glFogfv(GL_FOG_COLOR, BLANCO);
	glFogf(GL_FOG_DENSITY, 0.2);

	//culling
	glCullFace(GL_BACK);

	//blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//luces
	glLightfv(GL_LIGHT0, GL_DIFFUSE, BLANCO);// hacer que sea una luz con color
	// en utilidades hay colores
	glEnable(GL_LIGHT0);

	//LUZ DE MINERO
	glLightfv(GL_LIGHT1, GL_DIFFUSE, BLANCO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, BLANCO);
	glEnable(GL_LIGHT1);

	//texturas
	glGenTextures(1, &tex0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	loadImageFile((char*)"R.jpg");

	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	loadImageFile((char*)"nubes.jpg");

	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	loadImageFile((char*)"ventana.png");

	//crear menu
	glutCreateMenu(onMenu);
	glutAddMenuEntry("ALAMBRICO", 0);
	glutAddMenuEntry("SOLIDO", 1);
	glutAddMenuEntry("DOBLE", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar el fondo y profundidad
	/* estos dos comandos hacen que cada display no dismunuya el pealo */
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila

	//poner un fondo estatico
	glBindTexture(GL_TEXTURE_2D, tex1);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_MIRRORED_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	texturarFondo();

	//LUZ MINERO
	//LUCES SOLIDARIAS A LA CAMARA
	GLfloat punto_luz_1[] = { 0,0,0,1 }; //posicion 4 = 1 puntual
	glLightfv(GL_LIGHT1, GL_POSITION, punto_luz_1);


	//situar y orientar origen
	gluLookAt(3, 3, 4, 0, 0, 0, 0, 1, 0);


	ejes();


	////POSICIONAR LUCES FIJAS DE LA ESCEN COMO FAROLAS
	//GLfloat posicion_luz_0[] = { 0,3,0,1 }; //cuarta posicion = 1 luz puntual
	//glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz_0);




	//Aplicar textura
	glBindTexture(GL_TEXTURE_2D, tex0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // el color se aplica a la textura

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // repetir texturas
	//mirror da la vuelta a la textura al pegarla
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // repetir texturas

	//PONERRR
	// Suelo
	glMaterialfv(GL_FRONT, GL_DIFFUSE, AZUL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLANCO);
	//falta otro materialfv

	glBegin(GL_POLYGON);

	glNormal3f(0, 1, 0); // normal de los 4 vertices

	glTexCoord2f(0, 0);
	glVertex3f(-1, 0, 1);

	glTexCoord2f(2, 0);
	glVertex3f(1, 0, 1);

	glTexCoord2f(2, 2);
	glVertex3f(1, 0, -1);

	glTexCoord2f(0, 2);
	glVertex3f(-1, 0, -1);

	glEnd();


	//Donut 1


	//generacion automatica de texturas por mapa esferico de entorno
	glBindTexture(GL_TEXTURE_2D, tex1);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	//tetera 2
	glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISOSCURO); //MATERIAL BAJO
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLANCO); //BRILLO
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0); // BRILLANTE

	glPushMatrix();
	glTranslatef(0, 0, -0.4);
	glColor3f(0, 1, 0);
	//ORDEN CAMBIADO
	glRotatef(giroyTV, 0, 1, 0); // eje local a tetera
	glRotatef(giroxTV, 1, 0, 0); // general escena
	if (modoDibujo == ALAMBRICO) {
		glutWireTorus(0.3, 0.5, 50, 50);
	}
	else if (modoDibujo == SOLIDO) {
		glutSolidTorus(0.3, 0.5, 50, 50);

	}
	else {
		glutSolidTorus(0.3, 0.5, 50, 50);
		glColor3f(1, 1, 1);
		glutWireTorus(0.3, 0.5, 50, 50);
	}
	glPopMatrix();

	//dibujo traslucidos
	glDepthMask(GL_FALSE);


	//MATERIAL DEL PRIMER DONUT
	GLfloat rojotrans[4] = { 1,0,0,0.5 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, rojotrans);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO); //BRILLO BAJO

	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1, 0, 0);
	glRotatef(giroxTR, 1, 0, 0); // general escena
	glRotatef(giroyTR, 0, 1, 0); // eje local a tetera
	if (modoDibujo == ALAMBRICO) {
		glutWireTorus(0.3, 0.5, 50, 50);
	}
	else if (modoDibujo == SOLIDO) {
		glutSolidTorus(0.3, 0.5, 50, 50);

	}
	else {
		glutSolidTorus(0.3, 0.5, 50, 50);
		glColor3f(1, 1, 1);
		glutWireTorus(0.3, 0.5, 50, 50);
	}
	glPopMatrix();


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
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // filtro magnificacion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //filtro minificacion
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // luces no afectan
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-1, -1, 0);

	glTexCoord2f(1, 0);
	glVertex3f(1, -1, 0);

	glTexCoord2f(1, 1);
	glVertex3f(1, 1, 0);

	glTexCoord2f(0, 1);
	glVertex3f(-1, 1, 0);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// *_______________________________________________

	glutSwapBuffers(); // indicar que han acabado las ordenes de dibujo
}

void reshape(GLint w, GLint h) {

	glViewport(0, 0, w, h); //utiliza toda el area de dibujo

	float razon_aspecto = float(w) / float(h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//defino una camara de perspectiva
	gluPerspective(20, razon_aspecto, 0.2, 10);
	// si pongo menor angulo se ve mas grande

}


void onKey(unsigned char tecla, int x, int i) {
	//Callback de atencion al teclado

	switch (tecla) {
	case 'a':
		modoDibujo = ALAMBRICO;
		break;
	case 's':
		modoDibujo = SOLIDO;
		break;
	case 'd':
		modoDibujo = DOBLE;
		break;
	}

	glutPostRedisplay(); // evento de redibujo con los cambios
}

void select() {
	//dibuja en el back_buffer la escena con solo los objetos que interesa seleccionar
	// cada uno de un color, y sin intercambiar con el buffer frontal

	//Apilar el estado del motor 
	glPushAttrib(GL_ALL_ATTRIB_BITS); // lo apila todo (enables, etc)

	glDisable(GL_TEXTURE_2D); // deshabilito las texturas
	glDisable(GL_LIGHTING); //deshabilitar la iluminacion para que funcione el gl_color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar el fondo y profundidad
	/* estos dos comandos hacen que cada display no dismunuya el pealo */
	glMatrixMode(GL_MODELVIEW); // selecciono model-view
	glLoadIdentity(); // carga la identidad en el tope de la pila

	//situar y orientar origen
	gluLookAt(3, 3, 4, 0, 0, 0, 0, 1, 0);

	// Posicionar las luces fijas de la escena
	GLfloat pl0[] = { 0,3,0, 1 };	// Luz puntual
	glLightfv(GL_LIGHT0, GL_POSITION, pl0);

	//donut 1
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1, 0, 0);
	glRotatef(giroxTR, 1, 0, 0); // general escena
	glRotatef(giroyTR, 0, 1, 0); // eje local a tetera

	glutSolidTorus(0.3, 0.5, 50, 50);
	glPopMatrix();

	//donut 2
	glPushMatrix();
	glTranslatef(0, 0, -0.4);
	glColor3f(0, 1, 0);
	//ORDEN CAMBIADO
	glRotatef(giroyTV, 0, 1, 0); // eje local a tetera
	glRotatef(giroxTV, 1, 0, 0); // general escena
	glutSolidTorus(0.3, 0.5, 50, 50);
	glPopMatrix();

	//glEnable(GL_LIGHTING); // lo volvemos a habilitar

	glPopAttrib(); // restaura estado motor

}

void onClick(int button, int state, int x, int y) {
	// Callback de atencion al click del raton
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
		dragging = true;

		//	quiero saber el color de x,y en el backbuffer habiendo dibujado con colores solidos 
		// diferentes las teteras
		// preguntar cuanto mide mi viewport
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		int vy = viewport[3] - y;

		select(); // dibuja en el backbuffer ("no se ve")

		glReadPixels(x, vy, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &esRoja);// x,vy dimensiones, canal a leer, como lo quiero leer
		glReadPixels(x, vy, 1, 1, GL_GREEN, GL_UNSIGNED_BYTE, &esVerde);// x,vy dimensiones, canal a leer, como lo quiero leer
	}
	else {
		dragging = false;
	}
}

void onDrag(int x, int y) {
	//callback de arrastre del raton

	static const float pixel2grados = 1.0;

	if (!dragging) { return; }

	if (esRoja == 0xFF) {
		giroxTR += (x - xanterior)* pixel2grados;
		giroyTR += (y - yanterior) * pixel2grados;
	}
	else if (esVerde == 0xFF) {
		giroxTV += (x - xanterior)* pixel2grados;
		giroyTV += (y - yanterior) * pixel2grados;
	}

	xanterior = x;
	yanterior = y;

	glutPostRedisplay();

}

void main(int argc, char** argv)
{
	//inicializar freeimage
	FreeImage_Initialise();

	glutInit(&argc, argv);
	// Buffers a usar (area de memoria para habilitar)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // buffer profundidad
	//Posicionar la ventana de dibujo en la pantalla
	glutInitWindowPosition(100, 200);
	// Ajustar tamanyo ventana
	glutInitWindowSize(600, 600);
	//Crear la ventana
	glutCreateWindow(PROYECTO); // Registrar las callbacks (funcion que se dispara cuando sucede algo)
	init();


	glutDisplayFunc(display);// Evento display -> Cuando hay que dibujar
	glutReshapeFunc(reshape);// Evento resize/reshape (cambio ventana)
	glutKeyboardFunc(onKey); // Evento pulsar tecla
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);


	std::cout << PROYECTO  " en marcha" << std::endl;
	//Poner en marcha el bucle de atencion a eventos
	glutMainLoop();//atiende eventos

	//LIBERAR RECURSOS freeimage
	FreeImage_DeInitialise();

}