#ifndef _TEXTURAS_H_
#define _TEXTURAS_H_

#include "Global.h"

#include "TEXTURAS-CARTELES.h" // texturas carteles
#include "TEXTURAS_ELEMENTOS_SOLIDARIOS.h"
#include "TEXTURAS-FONDO.h"
#include "TEXTURAS-SUELO.h"
#include "TEXTURAS_VEHICULOS.h"

void cargaTexturas() {
	cargaTexturasSuelo_y_Carretera();
	cargaTexturaFondo();
	cargaTexturasCarteles();
	cargaTexturasVehiculos();
	cargaTexturasElementosSolidarios();
}

#endif