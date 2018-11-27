//FICHERO MODIFICADO-E1
//FICHERO MODIFICADO-E2
// GUARDADO EN .\src\

/*
	@classPrefix SUPPORT_
	@author Angel Igualada
	@version oct, 2018
*/

/*AÑADIDO PARTE ENTREGA1*/
#include <GameCharacters.h>
#include <SIMessage.h>
#include <GlobalDefs.h>
#include <SIGame.h>
#include <Support.h> // incluimos la cabecera de nuestra clase
#include <atlstr.h>  // incluyo clase para sacar MessageBox

//extern CShootsManager	*ShootsManager;
extern CSIGame			*Game;
CCharactersPool			*CharacterPool;

//VALORES DEFAULT ATRIBUTOS CREADOS
#define SUPPORT_DEFAULT_ALTURA	8
#define SUPPORT_DEFAULT_ESCUDO	25
#define SUPPORT_DEFAULT_FROZEN_TIME_AFTER_IMPACT 1000
#define SUPPORT_DEFAULT_PLAYER_CURATION 5
//VALORES DEFAULT OTROS ATRIBUTOS
#define SUPPORT_DEFAULT_HEALTH			100
#define SUPPORT_DEFAULT_ACCELERATION	0.01f
#define SUPPORT_DEFAULT_SHOOTSPEED		100
#define SUPPORT_DEFAULT_LIVES			1
#define SUPPORT_DEFAULT_HIT_DURATION	500

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         INIT                                                   //
//                         Da los valores por defecto a los atributos                             //
//            Returns true if the initialization of the attirbute has been correctly done         //
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSupport::Init(){

	Type = CHARS_SUPPORT; // SU TIPO ES EL DEL TOKEN QUE DEFINIMOS

	// DAMOS VALORES POR DEFECTO A LOS ATRIBUTOS QUE AÑADIMOS NUEVOS
	altura = SUPPORT_DEFAULT_ALTURA;
	escudo = SUPPORT_DEFAULT_ESCUDO;
	frozen_time_after_impact = SUPPORT_DEFAULT_FROZEN_TIME_AFTER_IMPACT;
	player_curation = SUPPORT_DEFAULT_PLAYER_CURATION;

	// DAMOS VALORES POR DEFECTO A OTROS ATRIBUTOS
	Lives = SUPPORT_DEFAULT_LIVES;
	Health = MaxHealth = SUPPORT_DEFAULT_HEALTH;
	Hit_duration = SUPPORT_DEFAULT_HIT_DURATION;
	ShootSpeed = SUPPORT_DEFAULT_SHOOTSPEED;
	acceleration = SUPPORT_DEFAULT_ACCELERATION;

	mostrarMensajeParser = true; // TRUE -> MOSTRAR VENTANAS PARSEADORES

	ResetScale(); //HEREDADO DE LA CLASE UGKCharacter pone la escala a (1,1,1)

	Rotation.v[XDIM] = 0.0;
	Rotation.v[YDIM] = 0.0;
	Rotation.v[ZDIM] = 0.0;

	Position.v[XDIM] = 10.0;
	Position.v[YDIM] = 10.0;
	Position.v[ZDIM] = 10.0;

	Speed.v[XDIM] = 0.0f;
	Acceleration.v[XDIM] = 0.0f;

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


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         RENDER                                                 //
//         Muestra al support por pantalla tomando sus valores de posicion y rotación actuales    //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CSupport::Render(void) {

	switch (RenderMode)
	{
	case CHAR_2D: break; //En esta version de SI no funciona -> no se implementa
	case CHAR_LINES3D: break;
	case CHAR_3D:
		Mesh->modelo.pos.v = Position.v; // a los lados -8 a 8

		Mesh->modelo.rot.v = Rotation.v;

		Mesh->modelo.scale.x = 1;
		Mesh->modelo.scale.y = 1;
		Mesh->modelo.scale.z = 1;

		Mesh->modelo.Draw();
		break;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         MUESTRAATRIBUTOS                                       //
// Si el atributo mostrarMensajeParser está a true saca una ventana con los atributos,al inicio   //
// y al final del initialization parser y el level parser para comprobar el parseado de los htmls //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CSupport::muestraAtributos(string tituloVentana) {
	if (mostrarMensajeParser) {
		std::stringstream ss;
		ss << "\tLives Valor Default: " << SUPPORT_DEFAULT_LIVES<< "  Valor actual: " << Lives << "\n";
		ss << "\tHealth Valor Default: " << SUPPORT_DEFAULT_HEALTH << "  Valor actual: " << Health << "\n";
		ss << "\tHit_duration Valor Default: " << SUPPORT_DEFAULT_HIT_DURATION << "  Valor actual: " << Hit_duration << "\n";
		ss << "\tShootSpeed Valor Default: " << SUPPORT_DEFAULT_HIT_DURATION << "  Valor actual: " << ShootSpeed << "\n";
		ss << "\tAcceleration Valor Default: " << SUPPORT_DEFAULT_ACCELERATION << "  Valor actual: " << acceleration << "\n";

		ss << "\n\tATRIBUTOS CREADOS\n";
		ss << "\tAltura Valor Default: " << SUPPORT_DEFAULT_ALTURA << "  Valor actual: " << altura << "\n";
		ss << "\tEscudo Valor Default: " << SUPPORT_DEFAULT_ESCUDO << "  Valor actual: " << escudo << "\n";
		ss << "\tFrozen_time_after_impact Valor Default: " << SUPPORT_DEFAULT_FROZEN_TIME_AFTER_IMPACT << "  Valor actual: " << frozen_time_after_impact << "\n";
		ss << "\tPlayer_curation Valor Default: " << SUPPORT_DEFAULT_PLAYER_CURATION << "  Valor actual: " << player_curation << "\n";

		ss << "\n\tATRIBUTOS VISUALIZACION\n";
		ss << "\tMESH Default: " << "Support.3ds" << "  Valor actual: " << MeshName << "\n";
		ss << "\tScale.v[XDIM] Valor Default: " << 1 << " Valor actual: " << Scale.v[XDIM] << "\n";
		ss << "\tScale.v[YDIM] Valor Default: " << 1 << " Valor actual: " << Scale.v[YDIM] << "\n";
		ss << "\tScale.v[ZDIM] Valor Default: " << 1 << " Valor actual: " << Scale.v[ZDIM] << "\n";
		ss << "\tPosition.v[XDIM] Valor Default: " << 10 << " Valor actual: " << Position.v[XDIM] << "\n";
		ss << "\tPosition.v[YDIM] Valor Default: " << 10 << " Valor actual: " << Position.v[YDIM] << "\n";
		ss << "\tPosition.v[ZDIM] Valor Default: " << 10 << " Valor actual: " << Position.v[ZDIM] << "\n";
		ss << "\tRotation.v[XDIM] Valor Default: " << 0 << " Valor actual: " << Rotation.v[XDIM] << "\n";
		ss << "\tRotation.v[YDIM] Valor Default: " << 0 << " Valor actual: " << Rotation.v[YDIM] << "\n";
		ss << "\tRotation.v[ZDIM] Valor Default: " << 0 << " Valor actual: " << Rotation.v[ZDIM] << "\n";
		CString msg = UGKS_string2CString(ss.str());
		const wstring tituloVentanaWStr = wstring(tituloVentana.begin(), tituloVentana.end());
		MessageBox(NULL, msg, tituloVentanaWStr.c_str(), MB_OK);
	}
}
