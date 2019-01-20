//FICHERO MODIFICADO-E1
//FICHERO MODIFICADO-E2
//FICHERO MODIFICADO-E3
// GUARDADO EN .\src\

/**
	@classPrefix SUPPORT_
	@author Angel Igualada
	@version oct, 2018
*/

/*AÑADIDO PARTE ENTREGA1*/
#include <GameCharacters.h>
#include <SIMessage.h>
#include <GlobalDefs.h>
#include <SIGame.h>
#include <Support.h> //Incluir la cabecera de nuestra clase
#include <atlstr.h>  //Incluir para sacar MessageBox
#include <UGKMeshManager.h> //Incluir para cambiar mallas
#include <ShootsManager.h> //Incluir para tener acceso a la lista de shoots y comprobar si colisionan 

extern CMeshesManager	MeshesManager; ///<Atributo tomado de Global.cpp que se utiliza para cambiar las mallas
extern CShootsManager	*ShootsManager; ///<Atributo tomado de Global.cpp que se utiliza para obtener la lista de los shoots activos
extern CSupport *Support_tras_level_parser; ///< Es el Support del level parser, con él pongo los valores de los atributos en la siguiente vida

//VALORES DEFAULT ATRIBUTOS CREADOS
#define SUPPORT_DEFAULT_ALTURA	999 ///<Altura por defecto 
#define SUPPORT_DEFAULT_ESCUDO	25  ///<Escudo por defecto 
#define SUPPORT_DEFAULT_FROZEN_TIME_AFTER_IMPACT 1000 ///<Periodo de congelacion tras disparo por defecto 
#define SUPPORT_DEFAULT_PLAYER_CURATION 5 ///<Cantidad de salud que se puede curar al player por defecto 
#define SUPPORT_DEFAULT_NUM_CURACIONES   2 ///<Numero de curaciones por defecto 
#define SUPPORT_DEFAULT_TIEMPO_ENTRE_CURACIONES 15000 ///<Tiempo en ms que deba transcurrir para poder curar de nuevo al player 
#define SUPPORT_DEFAULT_UMBRAL_PASAR_A_DAÑADO 20 ///<Umbral de salud defecto del support en el cual se considerará en estado danyado
#define SUPPORT_DEFAULT_UMBRAL_DAÑO_PLAYER 30 ///<Umbral de salud del player a partir del cual podra ser curado por defecto 
//VALORES DEFAULT OTROS ATRIBUTOS
#define SUPPORT_DEFAULT_HEALTH			100  ///<Salud del support por defecto 
#define SUPPORT_DEFAULT_LIVES			1    ///<Numero de vidas del support por defecto 
#define SUPPORT_DEFAULT_HIT_DURATION	500  ///<Hitduration del support por defecto



////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         INIT                                                   //
/**
* \fn bool CSupport::Init()
* \brief Asigna los valores por defecto a los atributos
* \return true si la inicializacion se ha hecho con exito
* \n false en otro caso
*/
bool CSupport::Init(){

	Type = CHARS_SUPPORT; // SU TIPO ES EL DEL TOKEN QUE DEFINIMOS

	// DAMOS VALORES POR DEFECTO A LOS ATRIBUTOS QUE AÑADIMOS NUEVOS
	altura = SUPPORT_DEFAULT_ALTURA;
	escudo = SUPPORT_DEFAULT_ESCUDO;
	frozen_time_after_impact = SUPPORT_DEFAULT_FROZEN_TIME_AFTER_IMPACT;
	player_curation = SUPPORT_DEFAULT_PLAYER_CURATION;
	num_curaciones = SUPPORT_DEFAULT_NUM_CURACIONES;
	tiempo_entre_curaciones = SUPPORT_DEFAULT_TIEMPO_ENTRE_CURACIONES;
	umbral_daño_player = SUPPORT_DEFAULT_UMBRAL_DAÑO_PLAYER;
	umbral_pasa_a_dañado = SUPPORT_DEFAULT_UMBRAL_PASAR_A_DAÑADO;

	// DAMOS VALORES POR DEFECTO A OTROS ATRIBUTOS
	Lives = SUPPORT_DEFAULT_LIVES;
	Health = MaxHealth = SUPPORT_DEFAULT_HEALTH;
	Hit_duration = SUPPORT_DEFAULT_HIT_DURATION;

	// DAMOS EL VALOR INICIAL AL ESTADO
	estado = SUPPORT_VIVO;

	//Collisionable(true);

	mostrarMensajeParser = false; // TRUE -> MOSTRAR VENTANAS PARSEADORES
	mostrarMensajeDisparoRecibido = false; // TRUE -> MOSTRAR VENTANAS IMPACTOS

	ResetScale(); // HEREDADO DE LA CLASE UGKCharacter pone la escala a (1,1,1)
	Scale.v[ZDIM] = 2;

	// FIJAMOS LA ROTACION INICIAL A (0, 0, 0)
	Rotation.v[XDIM] = 0.0;
	Rotation.v[YDIM] = 0.0;
	Rotation.v[ZDIM] = 0.0;

	// FIJAMOS LA POSICION INICIAL A (10, 10, 10)
	Position.v[XDIM] = 10.0;
	Position.v[YDIM] = 10.0;
	Position.v[ZDIM] = 10.0;

	Speed.v[XDIM] = 0.3f; // FIJAMOS LA VELOCIDAD EN EL EJE X A 0.3

	Immortal = false; // FIJAMOS INMMORTAL A FALSE

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         CONSTRUCTOR                                            //
/**
* \fn CSupport::CSupport()
* \brief Crea un Support y llama a init para dar los valores por defecto a los atributos
*/
CSupport::CSupport()
{
	Init(); //Llama a init()
	SetLocalTimers(NUMERO_DE_TIMERS_SUPPORT); //Fija los timers locales

	//Fija las alarmas de los timers
	Timer[PERIODO_CONGELACION].SetAlarm((double)frozen_time_after_impact); //Fija la alarma del timer del periodo de congelacion
	Timer[PERIODO_ENTRE_LAS_CURACIONES].SetAlarm((double)tiempo_entre_curaciones); //Fija la alarma del timer del periodo entre curaciones
	if (tiempo_entre_curaciones>5000) {
		Timer[PERIODO_PONER_MESH_NORMAL_PLAYER].SetAlarm((double)5000); //Fija la alarma del timer del periodo de malla "siendo curado" del player
	}
	else {
		Timer[PERIODO_PONER_MESH_NORMAL_PLAYER].SetAlarm((double)tiempo_entre_curaciones); //Fija la alarma del timer del periodo de malla "siendo curado" del player
	}
	//Activa los timers
	Timer[PERIODO_CONGELACION].Activate();//Activa el timer del periodo de congelacion
	Timer[PERIODO_ENTRE_LAS_CURACIONES].Activate();//Activa el timer del periodo entre curaciones
	Timer[PERIODO_PONER_MESH_NORMAL_PLAYER].Activate();//Activa el timer del periodo de malla "siendo curado" del player

	Timer[PERIODO_ENTRE_LAS_CURACIONES].InitCounting(); //Inicia la cuenta del timer del periodo entre curacione

}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         RENDER                                                 //
/**
* \fn void CSupport::Render(void)
* \brief Muestra al Support por pantalla.
*/
void CSupport::Render(void) {

	switch (RenderMode) //switch(Modo de render)
	{
	case CHAR_2D: break; //MODO 2D :En esta version de SI no funciona  (no se implementa)
	case CHAR_LINES3D: break;
	case CHAR_3D://MODO 3D:
		Mesh->modelo.pos.v = Position.v; //Fijamos la posicion de la mesh

		Mesh->modelo.rot.v = Rotation.v; //Fijamos la rotacion de la mesh

		Mesh->modelo.scale.v = Scale.v; //Fijamos la escala de la mesh

		Mesh->modelo.Draw(); //Dibujamos la mesh
		break;
	}

}



////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         UPDATE                                                 //
/**
* \fn void CSupport::Update(void)
* \brief Actualiza el Support.
*/
void CSupport::Update(void) {
	CPlayer* player = Players[CurrentPlayer];
	if (estado == SUPPORT_VIVO || estado == SUPPORT_DAÑADO) {
		if (altura != SUPPORT_DEFAULT_ALTURA && altura > -6 && altura < 6) {
			Position.v[YDIM] = altura;
		}
		float posicion_player_x = player->Position.v[XDIM];
		if (posicion_player_x > Position.v[XDIM] + 1.2) {
			Position.v[XDIM] += Speed.v[XDIM];
		}
		else if (posicion_player_x < Position.v[XDIM] - 1.2) {
			Position.v[XDIM] -= Speed.v[XDIM];
		}
	}

	if (Timer[PERIODO_PONER_MESH_NORMAL_PLAYER].IsSounding()) {
		cambiaMeshPlayer(PLAYER_NORMAL);
	}
	runLogic();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         CAMBIAMESHPLAYER                                       //
/**
* \fn void CSupport::cambiaMeshPlayer(ESTADO_PLAYER e)
* \brief Cambia la mesh del player.
* \param[in] e (ESTADO_PLAYER) Nuevo estado del Player.
*/
void CSupport::cambiaMeshPlayer(ESTADO_PLAYER e) {
	string nueva_mesh = "";
	if (e == PLAYER_NORMAL) {
		nueva_mesh = PLAYER_MESH;
	}
	else if (e == PLAYER_SIENDO_CURADO) {
		nueva_mesh = PLAYER_SIENDO_CURADO_MESH;
	}
	if (nueva_mesh != "") {
		int ind = MeshesManager.AddModel(nueva_mesh);
		Players[CurrentPlayer]->IndMesh = ind;
		Players[CurrentPlayer]->Mesh = MeshesManager.GetMesh(Players[CurrentPlayer]->IndMesh);
		SetMeshName(Players[CurrentPlayer]->Mesh->GetFileName());
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               CAMBIAMESH                                       //
/**
* \fn void CSupport::cambiaMesh(SUPPORT_STATE nuevo_estado)
* \brief Cambia la mesh del support segun su estado y su color predefinido.
* \param[in] nuevo_estado (SUPPORT_STATE) Nuevo estado del Support.
*/
void CSupport::cambiaMesh(SUPPORT_STATE nuevo_estado) {
	string nueva_mesh="";
	switch (color){
	case SUPPORT_COLOR_ROJO:
		switch (nuevo_estado)
		{
		case SUPPORT_DAÑADO: 
			nueva_mesh = SUPPORT_MESH_ROJO_DAÑO;
			break;
		case SUPPORT_CONGELADO:
			nueva_mesh = SUPPORT_MESH_CONGE;
			break;
		case SUPPORT_VIVO:
			if (escudo > 0) { nueva_mesh = SUPPORT_MESH_ROJO_ESCUDO; }
			else { nueva_mesh = SUPPORT_MESH_ROJO; }
			break;
		case SUPPORT_CONGELADO_Y_DAÑADO:
			nueva_mesh = SUPPORT_MESH_CONGE_DAÑO;
			break;
		default:
			break;
		}
		break;
	case SUPPORT_COLOR_GRIS:
		switch (nuevo_estado)
		{
		case SUPPORT_DAÑADO:
			nueva_mesh = SUPPORT_MESH_GRIS_DAÑO;
			break;
		case SUPPORT_CONGELADO:
			nueva_mesh = SUPPORT_MESH_CONGE;
			break;
		case SUPPORT_VIVO:
			if(escudo>0){ nueva_mesh = SUPPORT_MESH_GRIS_ESCUDO; }
			else{ nueva_mesh = SUPPORT_MESH_GRIS; }
			break;
		case SUPPORT_CONGELADO_Y_DAÑADO:
			nueva_mesh = SUPPORT_MESH_CONGE_DAÑO;
			break;
		default:
			break;
		}
		break;
	case SUPPORT_COLOR_AMARILLO:
		switch (nuevo_estado)
		{
		case SUPPORT_DAÑADO:
			nueva_mesh = SUPPORT_MESH_AMAR_DAÑO;
			break;
		case SUPPORT_CONGELADO:
			nueva_mesh = SUPPORT_MESH_CONGE;
			break;
		case SUPPORT_VIVO:
			if (escudo > 0) { nueva_mesh = SUPPORT_MESH_AMAR_ESCUDO; }
			else { nueva_mesh = SUPPORT_MESH_AMAR; }
			break;
		case SUPPORT_CONGELADO_Y_DAÑADO:
			nueva_mesh = SUPPORT_MESH_CONGE_DAÑO;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (nueva_mesh == "") {
		nueva_mesh = "support-amarillo.3ds";
	}
	int ind = MeshesManager.AddModel(nueva_mesh);
	IndMesh = ind;
	Mesh = MeshesManager.GetMesh(IndMesh);
	SetMeshName(Mesh->GetFileName());
	
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        DISPARO_RECIBIDO                                        //
/**
* \fn float CSupport::disparo_recibido(void)
* \brief Detecta si el Support recibe un disparo y devuelve el danyo.
* \return El daño recibido (Health del shoot).
*/
float CSupport::disparo_recibido(void) {
	for (auto i = 0; i < ShootsManager->ShootsAct.size(); i++)
	{
		if (abs(ShootsManager->ShootsAct[i]->Position.v[XDIM] - this->Position.v[XDIM])<0.6) { // podría haber colision
			if (abs(ShootsManager->ShootsAct[i]->Position.v[YDIM] - this->Position.v[YDIM]) < 0.1) {
				float saludShoot = ShootsManager->ShootsAct[i]->Health;
				ShootsManager->ShootsAct[i]->AI_Explode();
				if (mostrarMensajeDisparoRecibido) { muestraEstadoTrasImpacto(saludShoot); /*Support_tras_level_parser->muestraAtributos("atributos");*/ }
				return saludShoot;
			}
		}
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          RUNLOGIC                                              //
//             Ejecuta la logica del support dependiendo de en que estado se encuentre            //
/**
* \fn void CSupport::runLogic(void)
* \brief Ejecuta la logica del support dependiendo de en que estado se encuentre.
*/
void CSupport::runLogic(void) {
	CPlayer* player = Players[CurrentPlayer];
	float daño_recibido;
	std::stringstream ss;
	CString msg;
	switch (estado)
	{
	case SUPPORT_VIVO:
		daño_recibido = disparo_recibido();
		if (daño_recibido >0) {
			if (escudo > 0) {
				if (daño_recibido > escudo) {
					Health -= (daño_recibido - escudo);
					escudo = 0;
					Sound[SUPPORT_SONIDO_ESCUDO_ROTO]->Play(UGKSND_MAX_VOLUME);
				}
				else {
					Sound[SUPPORT_SONIDO_DISPARO_A_ESCUDO]->Play(UGKSND_MAX_VOLUME);
					escudo -= daño_recibido;
				}
			}
			else {
				Health -= daño_recibido;
			}	
			if (Health > 0) {
				if (escudo <= 0) {
					cambiaMesh(SUPPORT_CONGELADO);
					estado = SUPPORT_CONGELADO;
					Timer[PERIODO_CONGELACION].InitCounting();
				}
			}else {
				Sound[SUPPORT_SONIDO_MURIENDO]->Play(UGKSND_ULTRA_LOW_VOLUME);
				estado = SUPPORT_MURIENDO;
			}	
		}
		if (player->Health <= umbral_daño_player) {
			if (num_curaciones > 0 && Timer[PERIODO_ENTRE_LAS_CURACIONES].IsSounding()) {
				Sound[SUPPORT_SONIDO_CURAR_PLAYER]->Play(UGKSND_ULTRA_LOW_VOLUME);
				Players[CurrentPlayer]->Health += player_curation;
				num_curaciones--;
				cambiaMeshPlayer(PLAYER_SIENDO_CURADO);
				Timer[PERIODO_PONER_MESH_NORMAL_PLAYER].InitCounting();
				Timer[PERIODO_ENTRE_LAS_CURACIONES].InitCounting();
			}
		}
		break;
	case SUPPORT_MURIENDO:
		Scale.v[XDIM] *= 0.99;
		Scale.v[YDIM] *= 0.99;
		Scale.v[ZDIM] *= 0.99;
		Rotation.v[ZDIM] += 3;
		Position.v[ZDIM] += 0.05;
		if (Scale.v[XDIM]<=0.1) {
			estado = SUPPORT_MUERTO;
		}
		//audio muriendo
		break;
	case SUPPORT_MUERTO:
		
		if (Lives > 1) {
			//audio vuelta_vida
			Lives=Lives-1;
			reinicia_los_valores_level_parser();
			cambiaMesh(SUPPORT_VIVO);
			Sound[SUPPORT_SONIDO_REAPARECER]->Play(UGKSND_ULTRA_LOW_VOLUME);
		}
		else {
			Position.v[YDIM] = 100;
		}
		break;
	case SUPPORT_DAÑADO:
		daño_recibido = disparo_recibido();
		if (daño_recibido > 0) {
			//audio disparo
			Health -= daño_recibido;
			if (Health > 0) {
				cambiaMesh(SUPPORT_CONGELADO_Y_DAÑADO);
				estado = SUPPORT_CONGELADO_Y_DAÑADO;
				Timer[PERIODO_CONGELACION].InitCounting();
			}else if (Health <= 0) {
				Sound[SUPPORT_SONIDO_MURIENDO]->Play(UGKSND_ULTRA_LOW_VOLUME);
				estado = SUPPORT_MURIENDO;
			}
		}else if (player->Health<= umbral_daño_player) {
			if (num_curaciones > 0 && Timer[PERIODO_ENTRE_LAS_CURACIONES].IsSounding()) {
				Sound[SUPPORT_SONIDO_CURAR_PLAYER]->Play(UGKSND_ULTRA_LOW_VOLUME);
				Players[CurrentPlayer]->Health += player_curation;
				num_curaciones--;
				cambiaMeshPlayer(PLAYER_SIENDO_CURADO);
				Timer[PERIODO_PONER_MESH_NORMAL_PLAYER].InitCounting();
				Timer[PERIODO_ENTRE_LAS_CURACIONES].InitCounting();
			}
		}
		break;
	case SUPPORT_CONGELADO:
		daño_recibido = disparo_recibido();
		if (daño_recibido > 0) {
			Health -= daño_recibido;
			if (Health > 0) {
				if (Health <= umbral_pasa_a_dañado) {
					cambiaMesh(SUPPORT_CONGELADO_Y_DAÑADO); // revisar
					estado = SUPPORT_CONGELADO_Y_DAÑADO;
				}
			}else if (Health <= 0) {
				Sound[SUPPORT_SONIDO_MURIENDO]->Play(UGKSND_ULTRA_LOW_VOLUME);
				estado = SUPPORT_MURIENDO;
			}
		}
		if (Timer[PERIODO_CONGELACION].IsSounding()) {
			if (Health > umbral_pasa_a_dañado ) {
				estado = SUPPORT_VIVO;
				cambiaMesh(SUPPORT_VIVO);
			}
			else {
				estado = SUPPORT_DAÑADO;
				cambiaMesh(SUPPORT_DAÑADO);
			}
		}
		break;
	case SUPPORT_CONGELADO_Y_DAÑADO:
		if (Timer[PERIODO_CONGELACION].IsSounding()) {
			estado = SUPPORT_DAÑADO;
			cambiaMesh(SUPPORT_DAÑADO);	
		}
		daño_recibido = disparo_recibido();
		if (daño_recibido > 0) {
			Health -= daño_recibido;
			 if (Health <= 0) {
				 Sound[SUPPORT_SONIDO_MURIENDO]->Play(UGKSND_ULTRA_LOW_VOLUME);
				 estado = SUPPORT_MURIENDO;
			 }
		}
		break;
	default:
		break;
	}

}





////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         COLLIDED                                               //
/**
* \fn void CSupport::Collided(CCharacter *CollidedChar)
* \brief Realiza la deteccion de colisiones.
* \param[in] *CollidedChar (CCharacter) Puntero al personaje con el que ha habido colision.
*/
void CSupport::Collided(CCharacter *CollidedChar)
{
	unsigned int i;

	if (Alive() && !Immortal)
	{
		//Sound[CPL_TOUCH_SND]->Play(UGKSND_VOLUME_60);
	}
	else return;

	if (CHARS_COLLISION_TABLE[CHARS_SUPPORT][CollidedChar->Type]) {
		switch (CollidedChar->Type)
		{
		case CHARS_SHOOT: 
			switch (((CShoot*)CollidedChar)->SubType)
			{
			case CSH_PLAYER2D:
			case CSH_PLAYER3D:
			case CSH_AUX_LASER:
			case CSH_PLAYER3D_CHEVRON:
			case CSH_SHIP:				//Shoot sent by a enemy ship. This kind of shoot has to be always after all player's shootings
			case CSH_SUPPLY_SHIP:		//Shoot sent by a enemy Supply Ship. This kind of shoot has to be always after all player's shootings
			case CSH_CIRCLE_SHIP:
				Health -= CollidedChar->Health;
				Hit_duration--;
			}
			break;
		case CHARS_LASER:
			Health -= CollidedChar->Health;
			Hit_duration--;
			break;
		}
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                REINICIA_LOS_VALORES_LEVEL_PARSER                               //
/**
* \fn void CSupport::reinicia_los_valores_level_parser(void)
* \brief Reasigna los valores leidos en el levelParser al Support.
*/
void CSupport::reinicia_los_valores_level_parser(void) {
	Health = Support_tras_level_parser->Health;
	escudo = Support_tras_level_parser->escudo;
	estado = Support_tras_level_parser->estado;
	Position.v = Support_tras_level_parser->Position.v;
	Rotation.v = Support_tras_level_parser->Rotation.v;
	Scale.v = Support_tras_level_parser->Scale.v;
}




////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         MUESTRAATRIBUTOS                                       //
/**
* \fn void CSupport::muestraAtributos(string tituloVentana)
* \brief Lanza una ventana con los valores de los atributos del Support.
* \param[in] tituloVentana (string) para fijar el nombre de la ventana y saber en que punto se disparó el mensaje.
*/
void CSupport::muestraAtributos(string tituloVentana) {
	if (mostrarMensajeParser) {
		std::stringstream ss;
		ss << "\tLives Valor Default: " << SUPPORT_DEFAULT_LIVES<< "  Valor actual: " << Lives << "\n";
		ss << "\tHealth Valor Default: " << SUPPORT_DEFAULT_HEALTH << "  Valor actual: " << Health << "\n";
		ss << "\tHit_duration Valor Default: " << SUPPORT_DEFAULT_HIT_DURATION << "  Valor actual: " << Hit_duration << "\n";

		ss << "\n\tATRIBUTOS CREADOS\n";
		ss << "\tAltura Valor Default: " << SUPPORT_DEFAULT_ALTURA << "  Valor actual: " << altura << "\n";
		ss << "\tEscudo Valor Default: " << SUPPORT_DEFAULT_ESCUDO << "  Valor actual: " << escudo << "\n";
		ss << "\tFrozen_time_after_impact Valor Default: " << SUPPORT_DEFAULT_FROZEN_TIME_AFTER_IMPACT << "  Valor actual: " << frozen_time_after_impact << "\n";
		ss << "\tPlayer_curation Valor Default: " << SUPPORT_DEFAULT_PLAYER_CURATION << "  Valor actual: " << player_curation << "\n";
		ss << "\tSupport Color Valor Default: " << "" << " Valor actual: " << color << "\n";
		ss << "\tSupport numCurations Valor Default: " << SUPPORT_DEFAULT_NUM_CURACIONES << " Valor actual: " << num_curaciones << "\n";
		ss << "\tSupport umbral_player Valor Default: " << SUPPORT_DEFAULT_UMBRAL_DAÑO_PLAYER << " Valor actual: " << umbral_daño_player << "\n";
		ss << "\tSupport umbral_support Valor Default: " << SUPPORT_DEFAULT_UMBRAL_PASAR_A_DAÑADO << " Valor actual: " << umbral_pasa_a_dañado << "\n";
		ss << "\tSupport tiempo_entre_curaciones Valor Default: " << SUPPORT_DEFAULT_TIEMPO_ENTRE_CURACIONES << " Valor actual: " << tiempo_entre_curaciones << "\n";

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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                               MUESTRA_ESTADO_TRAS_IMPACTO                                      //
/**
* \fn void CSupport::muestraEstadoTrasImpacto(float saludShoot)
* \brief Lanza una ventana con los datos del disparo recibido.
* \param[in] saludShoot (float) Salud del shoot con el que colisiono.
*/
void CSupport::muestraEstadoTrasImpacto(float saludShoot) {
	std::stringstream ss;
	ss << "Salud antes de recibir el disparo  " << Health << "\n";
	ss << "Escudo antes de recibir el disparo  " << escudo << "\n";
	ss << "Estado antes de recibir el disparo  " << estado << "\n";
	ss << "Health shoot: " << saludShoot << "\n\n\n";
	ss << "LEYENDA Estados:\n";
	ss << " 0 -> VIVO \t 1-> MURIENDO \t2-> DAÑADO\n";
	ss << " 3 -> DAÑADO \t 4-> CONGELADO \t5-> CONGELADO Y DAÑADO\n";
	
	CString msg = UGKS_string2CString(ss.str());
	MessageBox(NULL, msg, L"DISPARO RECIBIDO", MB_OK);
}
