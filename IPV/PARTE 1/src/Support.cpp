//FICHERO MODIFICADO-E1
// GUARDADO EN .\src\
/*/////////////////////////////////////////////////////////////////////////////////////
//
// Player initialization or constructor
//
// These values are by default. They have to be overwritten in the initializatio phase
// when reading the default values from the "initialization.html" file
/*
@author Angel Igualada
@version 2018 - 10 - 12

*/
#include <Glee\GLee.h>

#include <Laser.h>
/*AÑADIDO PARTE ENTREGA1*/
#include <Support.h>
#include <GameCharacters.h>
#include <SIMessage.h>
#include <ShootsManager.h>	/// Header File class Manager for the shoots
#include <GlobalDefs.h>
#include <SIKeyboard.h>
#include <SIGame.h>

#define CPL_DEFAULT_LIVES		  1; ///<If not specified, by default, ther are three lives
#define CPL_MAX_HEALTH			 5;	///<Maximun amount of health of the SUPPORT by default
#define CPL_MAX_MAX_HIT_DUR		500	///<Maximun amount of time when the player is hit

#define CPL_BOUNDARY_MARGIN		1.0f
#define CPL_BOUNDARY_FRICTION	0.01f
#define CPL_VISCOUS_COEFF		0.002f

#define CPL_MIN_SPEED			0.0002f	//Minimun X speed in units/ms
#define CPL_MAX_SPEED			0.05f	//Maximun X speed in units/ms
#define CPL_MIN_ACCEL			0.0001f	//Minimun X acceleration in units/ms
#define CPL_MAX_ACCEL			0.0004f	//Maximun X acceleration in units/ms

#define CPL_2D_SIZE_X			0.3f	//
#define CPL_2D_SIZE_Y			0.2f	//
#define CPL_2D_SIZE_Z			0.1f	//

#define CPL_3D_SIZE_X			2.0f	//
#define CPL_3D_SIZE_Y			1.0f	//
#define CPL_3D_SIZE_Z			0.5f	//

#define CPL_LASER_XSHIFT		0.05f	///<Left or right laser relative position respect to the player on the X axis
#define CPL_REACTOR_XSHIFT		0.23f	///<Left or right reactor relative position respect to the player on the X axis
#define CPL_REACTOR_YSHIFT		0.30f	///<Left or right reactor relative position respect to the player on the Y axis
#define CPL_REACTOR_ZSHIFT		0.0f	///<Left or right reactor relative position respect to the player on the Z axis

//Shooting
#define CPL_SHOOT2D_SPEED		0.006f	//In units per milisecond
#define CPL_SHOOT3D_SPEED		0.005f	//In units per milisecond

extern CShootsManager	*ShootsManager;
extern CSIGame			*Game;
CCharactersPool			*CharacterPool;

//MODIFICACION PARTE ENTREGA1
#define SUPPORT_DEFAULT_xi		0.00;
#define SUPPORT_DEFAULT_yi		0.00;
#define SUPPORT_DEFAULT_zi		0.00;
#define SUPPORT_DEFAULT_xi_speed 0.01f;
#define SUPPORT_DEFAULT_yi_speed 0.01f;
#define SUPPORT_DEFAULT_zi_speed 0.01f;
#define SUPPORT_DEFAULT_altura	8;
#define SUPPORT_DEFAULT_escudo	25;
#define SUPPORT_DEFAULT_frozen_time_after_impact 1000;
#define SUPPORT_DEFAULT_player_curation 5;
#define SUPPORT_DEFAULT_health			100;
#define SUPPORT_DEFAULT_acceleration	0.01f;
#define SUPPORT_DEFAULT_ShootSpeed		100;
#define SUPPORT_DEFAULT_Lives			3;
#define SUPPORT_DEFAULT_Hit_duration	500;


bool CSupport::Init()	//Used when all the values are initialized by default
						//when reading the global initialization game file. 
						//Returns true if the initialization of the attirbute has been correctly done
{
	Type = CHARS_SUPPORT;
	Lives = SUPPORT_DEFAULT_Lives;
	Health = MaxHealth = SUPPORT_DEFAULT_health;
	Hit_duration = SUPPORT_DEFAULT_Hit_duration;

	UpdateSF(TimerManager.GetSF());

	//AÑADIDO ENTREGA1
	xi = SUPPORT_DEFAULT_xi;
	xi_speed = SUPPORT_DEFAULT_xi_speed;
	yi = SUPPORT_DEFAULT_yi;
	yi_speed = SUPPORT_DEFAULT_yi_speed;
	zi = SUPPORT_DEFAULT_zi;
	zi_speed = SUPPORT_DEFAULT_zi_speed;
	altura = SUPPORT_DEFAULT_altura;
	escudo = SUPPORT_DEFAULT_escudo;
	frozen_time_after_impact = SUPPORT_DEFAULT_frozen_time_after_impact;
	player_curation = SUPPORT_DEFAULT_player_curation;
	ShootSpeed = SUPPORT_DEFAULT_ShootSpeed;
	acceleration = SUPPORT_DEFAULT_acceleration;



	ResetScale();



#ifdef CHAR_USE_AABB
	UpdateAABB(CPL_2D_SIZE_X, CPL_2D_SIZE_Y, CPL_2D_SIZE_Z);
#endif

	Speed.v[XDIM] = 0.0f;
	Acceleration.v[XDIM] = 0.0f;

	Passing2D23D = false;
	DiscreteSimulation = false;

	Direction = 0.0f;

	//State = CPL_UNBORN;

	Immortal = false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Player Initialisation 
//
//

CSupport::CSupport()
{
	Init();

}
void CSupport::AI_Init(void) {}	///<Default Artificial Intelligence character initialization
void CSupport::AI_Explode(void) {}
void CSupport::AI_Die(void) {}
void CSupport::ActivateLaser(void) {}
void CSupport::Collided(CCharacter *CollidedChar) {}//Physics
void CSupport::Render(void) {}///Shows the support on the screen
void CSupport::RenderExplosion(void) {}
void CSupport::MoveTo(float x, float y, float z) {}	///<Moves to the absolute coordinate x,y,z
void CSupport::SupportShoot() {}
