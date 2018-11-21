//FICHERO MODIFICADO-E1  
// GUARDADO EN .\include\

/*
	@classPrefix SUPPORT_
	@author Angel Igualada
	@version oct, 2018
*/

#ifndef CLASS_SUPPORT
#define CLASS_SUPPORT

#include <Shoot.h>
#include <Reactor.h>
#include <Laser.h>


#define SUPPORT_Y_2D			SIGLBD_PG_BOTTOM + 8.0f
#define SUPPORT_Y_3D			SIGLBD_PG_BOTTOM + 8.0f

class CSupport : public CExplosiveChar
{
	//Attributes
public:
	//ATRIBUTOS AÑADIDOS
	float		altura;
	float		escudo;
	short int	frozen_time_after_impact;
	float		player_curation;

	//ATRIBUTOS COPIADOS DEL PLAYER
	short int	Lives;		///<Amount of lives 
	float       health;
	float		acceleration;
	float		xi;			// Posicion en x
	float		xi_speed;	// Velocidad desplazamiento x
	float		yi;			// Posicion en y
	float		yi_speed;	// Velocidad desplazamiento y
	float		zi;			// Posicion en z
	float		zi_speed;	// Velocidad desplazamiento z
	float		ShootSpeed;	///<Initial speed of the current shoot

	//ATRIBUTOS QUE USA SPACE INVADERS POR VERSIONES ANTERIORES PERO NO IMPORTAN
	bool			Passing2D23D;	///< Flag that prevents any destruction of the player, lasers,... while moving from 2D rendering mode to 3D one
	bool			DiscreteSimulation;
	float			Direction;		//-1.0 to the left and +1.0 to the right

	//ATRIBUTOS INFORMACION


	//Methods

	/*  Used when all the values are initialized by default when reading the global initialization game file
	 Returns true if the initialization of the attirbute has been correctly done   */
	bool Init();	

	/* CONSTRUCTOR */
	CSupport();		

	/* MUESTRA MENSAJE CON LOS ATRIBUTOS */
	void muestraAtributos(string tituloVentana);
};


#endif
