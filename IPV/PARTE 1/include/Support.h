//FICHERO MODIFICADO-E1  
// GUARDADO EN .\include\
//Definition of the class Player
/*Class prefix CPL_

	@author Ramon Molla
	@version 2011 - 09 - 11
*/

#ifndef CPL_CLASS_SUPPORT
#define CPL_CLASS_SUPPORT

#include <Shoot.h>
#include <Reactor.h>
#include <Laser.h>


#define CPL_SUPPORT_Y_2D			SIGLBD_PG_BOTTOM + 8.0f
#define CPL_SUPPORT_Y_3D			SIGLBD_PG_BOTTOM + 8.0f


//typedef enum {
//	CPL_UNBORN = 0,	///<For management purpouses only
//	CPL_BORN,		///<The character is just born but it is not still operative (living)
//	CPL_LIVING,		///<Everything this character may do while it is alive 
//	CPL_DYING,		///<The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
//	CPL_MOVING,		///<The player is moving to the left or to the right
//	CPL_SPEEDINGUP, ///<Player is moving the 
//	CPL_SHOOTING,	///<The player is shooting to the space invaders
//	CPL_DEAD,		///<The character is no operative. Reached after dying
//	CPL_MAXSTATE	///<For management purpouses only
//} CPL_SUPPORT_STATE;

class CSupport : public CExplosiveChar
{
	//Attributes
public:
	short int	Lives;		///<Amount of lives 
	float		altura;
	float		escudo;
	short int	frozen_time_after_impact;
	float		player_curation;
	float     health;
	float		acceleration;
	float		xi;			// Posicion en x
	float		xi_speed;	// Velocidad desplazamiento x
	float		yi;			// Posicion en y
	float		yi_speed;	// Velocidad desplazamiento y
	float		zi;			// Posicion en z
	float		zi_speed;	// Velocidad desplazamiento z
	float			ShootSpeed;	///<Initial speed of the current shoot

	//CPL_SUPPORT_STATE State;
	bool			Passing2D23D,	///< Flag that prevents any destruction of the player, lasers,... while moving from 2D rendering mode to 3D one
		DiscreteSimulation;
	float			Direction;		//-1.0 to the left and +1.0 to the right

	//Methods
	bool Init();	//Used when all the values are initialized by default
					//when reading the global initialization game file
					//Returns true if the initialization of the attirbute has been correctly done

	CSupport();		//Constructor


	///Character AI virtual methods rewritten
	void AI_Init(void);	///<Default Artificial Intelligence character initialization
	void AI_Explode(void);
	void AI_Die(void);
	void ActivateLaser(void);
	void Collided(CCharacter *CollidedChar);//Physics
	void Render(void);///Shows the support on the screen
	void RenderExplosion(void);
	void MoveTo(float x, float y, float z);	///<Moves to the absolute coordinate x,y,z
	void SupportShoot();

};


#endif
