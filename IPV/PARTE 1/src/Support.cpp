
//FICHERO MODIFICADO-E1
// GUARDADO EN .\src\

/*
	@classPrefix SUPPORT_
	@author Angel Igualada
	@version oct, 2018
*/


//#include <Glee\GLee.h>
//#include <Laser.h>
//#include <SIKeyboard.h>
//#include <ShootsManager.h>	/// Header File class Manager for the shoots

/*A�ADIDO PARTE ENTREGA1*/
#include <GameCharacters.h>
#include <SIMessage.h>
#include <GlobalDefs.h>
#include <SIGame.h>
#include <Support.h> // incluimos la cabecera de nuestra clase
#include <atlstr.h>  // incluyo clase para sacar MessageBox

#define SUPPORT_DEFAULT_LIVES		  1; ///<If not specified, by default, ther are three lives
#define SUPPORT_MAX_HEALTH			 5;	///<Maximun amount of health of the SUPPORT by default
#define SUPPORT_MAX_MAX_HIT_DUR		500	///<Maximun amount of time when the player is hit

#define SUPPORT_BOUNDARY_MARGIN		1.0f
#define SUPPORT_BOUNDARY_FRICTION	0.01f
#define SUPPORT_VISCOUS_COEFF		0.002f

#define SUPPORT_MIN_SPEED			0.0002f	//Minimun X speed in units/ms
#define SUPPORT_MAX_SPEED			0.05f	//Maximun X speed in units/ms
#define SUPPORT_MIN_ACCEL			0.0001f	//Minimun X acceleration in units/ms
#define SUPPORT_MAX_ACCEL			0.0004f	//Maximun X acceleration in units/ms

#define SUPPORT_2D_SIZE_X			0.3f	//
#define SUPPORT_2D_SIZE_Y			0.2f	//
#define SUPPORT_2D_SIZE_Z			0.1f	//

#define SUPPORT_3D_SIZE_X			2.0f	//
#define SUPPORT_3D_SIZE_Y			1.0f	//
#define SUPPORT_3D_SIZE_Z			0.5f	//

#define SUPPORT_LASER_XSHIFT		0.05f	///<Left or right laser relative position respect to the player on the X axis
#define SUPPORT_REACTOR_XSHIFT		0.23f	///<Left or right reactor relative position respect to the player on the X axis
#define SUPPORT_REACTOR_YSHIFT		0.30f	///<Left or right reactor relative position respect to the player on the Y axis
#define SUPPORT_REACTOR_ZSHIFT		0.0f	///<Left or right reactor relative position respect to the player on the Z axis

//Shooting
#define SUPPORT_SHOOT2D_SPEED		0.006f	//In units per milisecond
#define SUPPORT_SHOOT3D_SPEED		0.005f	//In units per milisecond

//extern CShootsManager	*ShootsManager;
extern CSIGame			*Game;
CCharactersPool			*CharacterPool;


//VALORES DEFAULT ATRIBUTOS CREADOS
#define SUPPORT_DEFAULT_altura	8;
#define SUPPORT_DEFAULT_escudo	25;
#define SUPPORT_DEFAULT_frozen_time_after_impact 1000;
#define SUPPORT_DEFAULT_player_curation 5;
//VALORES DEFAULT OTROS ATRIBUTOS
#define SUPPORT_DEFAULT_xi		0.00;
#define SUPPORT_DEFAULT_yi		0.00;
#define SUPPORT_DEFAULT_zi		0.00;
#define SUPPORT_DEFAULT_xi_speed 0.01f;
#define SUPPORT_DEFAULT_yi_speed 0.01f;
#define SUPPORT_DEFAULT_zi_speed 0.01f;
#define SUPPORT_DEFAULT_health			100;
#define SUPPORT_DEFAULT_acceleration	0.01f;
#define SUPPORT_DEFAULT_ShootSpeed		100;
#define SUPPORT_DEFAULT_Lives			1;
#define SUPPORT_DEFAULT_Hit_duration	500;


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         INIT                                                   //
//                         Da los valores por defecto a los atributos                             //
//            Returns true if the initialization of the attirbute has been correctly done         //
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSupport::Init(){

	Type = CHARS_SUPPORT; // SU TIPO ES EL DEL TOKEN QUE DEFINIMOS

	// DAMOS VALORES POR DEFECTO A LOS ATRIBUTOS QUE A�ADIMOS NUEVOS
	altura = SUPPORT_DEFAULT_altura;
	escudo = SUPPORT_DEFAULT_escudo;
	frozen_time_after_impact = SUPPORT_DEFAULT_frozen_time_after_impact;
	player_curation = SUPPORT_DEFAULT_player_curation;

	// DAMOS VALORES POR DEFECTO A OTROS ATRIBUTOS
	Lives = SUPPORT_DEFAULT_Lives;
	Health = MaxHealth = SUPPORT_DEFAULT_health;
	Hit_duration = SUPPORT_DEFAULT_Hit_duration;
	xi = SUPPORT_DEFAULT_xi;
	xi_speed = SUPPORT_DEFAULT_xi_speed;
	yi = SUPPORT_DEFAULT_yi;
	yi_speed = SUPPORT_DEFAULT_yi_speed;
	zi = SUPPORT_DEFAULT_zi;
	zi_speed = SUPPORT_DEFAULT_zi_speed;
	ShootSpeed = SUPPORT_DEFAULT_ShootSpeed;
	acceleration = SUPPORT_DEFAULT_acceleration;

	UpdateSF(TimerManager.GetSF()); //HEREDADO DE LA CLASE UGKCharacter inicia los timers


	ResetScale(); //HEREDADO DE LA CLASE UGKCharacter pone la escala a (1,1,1)

	Rotation.v[XDIM] = 0.0;
	Rotation.v[YDIM] = 0.0;
	Rotation.v[ZDIM] = 180.0;

	Position.v[XDIM] = 10.0;
	Position.v[YDIM] = 10.0;
	Position.v[ZDIM] = 10.0;

#ifdef CHAR_USE_AABB_SUPPORT
UpdateAABB(SUPPORT_2D_SIZE_X, SUPPORT_2D_SIZE_Y, SUPPORT_2D_SIZE_Z);
#endif

	Speed.v[XDIM] = 0.0f;
	Acceleration.v[XDIM] = 0.0f;

	Passing2D23D = false;
	DiscreteSimulation = false;

	Direction = 0.0f;

	Immortal = false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         CONSTRUCTOR                                            //
// Cuando se crea un Support, se llama a init para dar los valores por defecto a los atributos    //
////////////////////////////////////////////////////////////////////////////////////////////////////
CSupport::CSupport()
{
	Init();

}

void CSupport::muestraAtributos(string tituloVentana) {
	std::stringstream ss;
	ss << "\tType  Valor Default: " << CHARS_SUPPORT <<"  Valor actual: "<< Type << "\n";
	ss << "\tLives Valor Default: " << 1 << "  Valor actual: " << Lives << "\n";
	ss << "\tHealth Valor Default: " << 100 << "  Valor actual: " << Health << "\n";
	ss << "\tHit_duration Valor Default: "<< 500 << "  Valor actual: "<< Hit_duration << "\n";
	ss << "\txi Valor Default: " << 0<<"  Valor actual: "<< xi << "\n";
	ss << "\txi_speed Valor Default: "<< 0.01 <<"  Valor actual: " << xi_speed << "\n";
	ss << "\tyi Valor Default: " << 0 << "  Valor actual: "<< yi << "\n";
	ss << "\tyi_speed Valor Default: " << 0.01 << "  Valor actual: " << yi_speed << "\n";
	ss << "\tzi Valor Default: " << 0 << "  Valor actual: " << zi << "\n";
	ss << "\tzi_speed Valor Default: " << 0.01 << "  Valor actual: " << zi_speed << "\n";
	ss << "\tShootSpeed Valor Default: " << 100 << "  Valor actual: " << ShootSpeed << "\n";
	ss << "\tAcceleration Valor Default: " << 0.01 << "  Valor actual: " << acceleration << "\n";

	ss << "\n\tATRIBUTOS CREADOS\n";
	ss << "\tAltura Valor Default: " << 8 << "  Valor actual: " << altura << "\n";
	ss << "\tEscudo Valor Default: " << 25 << "  Valor actual: " << escudo << "\n";
	ss << "\tFrozen_time_after_impact Valor Default: " << 1000 << "  Valor actual: " << frozen_time_after_impact << "\n";
	ss << "\tPlayer_curation Valor Default: " << 5 << "  Valor actual: " << player_curation << "\n";
	ss << "\n\tATRIBUTOS VISUALIZACION\n";
	ss << "\tMESH Default: " << "Support.3ds" << "  Valor actual: " <<MeshName << "\n";
	ss << "\tIndAnimation2D Valor Default: " << 8 << "  Valor actual: " << IndAnimation2D << "\n";
	ss << "\tIndMesh Valor Default: " << 8 << "  Valor actual: " << IndMesh << "\n";
	ss << "\tIndTexture2D Valor Default: " << 8 << "  Valor actual: " << IndTexture2D << "\n";
	ss << "\tIndTexture3D Valor Default: " << 8 << "  Valor actual: " << IndTexture3D << "\n";
	ss << "\tRenderMode Valor Default: " << 8 << "  Valor actual: " << RenderMode << "\n";
	ss << "\tScale.v[XDIM] Valor Default: " << 1 <<" Valor actual: " << Scale.v[XDIM] << "\n";
	ss << "\tScale.v[YDIM] Valor Default: " << 1 <<" Valor actual: " << Scale.v[YDIM] << "\n";
	ss << "\tScale.v[ZDIM] Valor Default: " << 1 <<" Valor actual: " << Scale.v[ZDIM] << "\n";
	ss << "\tPosition.v[XDIM] Valor Default: " << 10 <<" Valor actual: " << Position.v[XDIM] << "\n";
	ss << "\tPosition.v[YDIM] Valor Default: " << 10 <<" Valor actual: " << Position.v[YDIM] << "\n";
	ss << "\tPosition.v[ZDIM] Valor Default: " << 10 <<" Valor actual: " << Position.v[ZDIM] << "\n";
	ss << "\tRotation.v[XDIM] Valor Default: " << 0 <<" Valor actual: " << Rotation.v[XDIM] << "\n";
	ss << "\tRotation.v[YDIM] Valor Default: " << 0 <<" Valor actual: " << Rotation.v[YDIM] << "\n";
	ss << "\tRotation.v[ZDIM] Valor Default: " << 180 <<" Valor actual: " << Rotation.v[ZDIM] << "\n";
	CString msg = UGKS_string2CString(ss.str());
	const wstring tituloVentanaWStr = wstring(tituloVentana.begin(), tituloVentana.end());
	MessageBox(NULL, msg, tituloVentanaWStr.c_str(), MB_OK);
}

