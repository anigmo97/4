#ifndef _NIEBLA_H_
#define _NIEBLA_H_

#include "Global.h"
extern bool niebla_activada;

void dibuja_niebla() {
	if (niebla_activada) {
		// Habilitamos niebla
		glEnable(GL_FOG);
		static GLfloat cniebla[] = { 1.0,1.0,1.0,1.0 }; // Color de la niebla
		glFogfv(GL_FOG_COLOR, cniebla);
		glFogf(GL_FOG_DENSITY, 0.02);
	}
}
#endif