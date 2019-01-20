//FICHERO MODIFICADO-E1
//FICHERO MODIFICADO-E3  
// GUARDADO EN .\include\

/**
	@classPrefix SUPPORT_
	@author Angel Igualada
	@version oct, 2018
*/

#ifndef CLASS_SUPPORT
#define CLASS_SUPPORT

#include <Shoot.h>
#include <Laser.h>
#include <Player.h> //añadido entrega3

#define SUPPORT_MESH_AMAR  "support-amarillo.3ds" ///<Malla support color amarillo
#define SUPPORT_MESH_ROJO  "support-rojo.3ds"     ///<Malla support color rojo
#define SUPPORT_MESH_GRIS  "support-gris.3ds"     ///<Malla support color gris

#define SUPPORT_MESH_AMAR_ESCUDO  "support-amarillo-escudo.3ds" ///<Malla support color amarillo con escudo
#define SUPPORT_MESH_ROJO_ESCUDO  "support-rojo-escudo.3ds"     ///<Malla support color rojo con escudo
#define SUPPORT_MESH_GRIS_ESCUDO  "support-gris-escudo.3ds"     ///<Malla support color gris con escudo

#define SUPPORT_MESH_AMAR_DAÑO  "support-amarillo-dañado.3ds"   ///<Malla support con grietas amarillo
#define SUPPORT_MESH_ROJO_DAÑO "support-rojo-dañado.3ds"        ///<Malla support con grietas rojo
#define SUPPORT_MESH_GRIS_DAÑO "support-gris-dañado.3ds"        ///<Malla support con grietas gris

#define SUPPORT_MESH_CONGE "support-congelado.3ds"              ///<Malla support congelado

#define SUPPORT_MESH_CONGE_DAÑO  "support-dañado-congelado.3ds" ///<Malla support congelado y dañado (con grietas)

#define PLAYER_MESH "player.3ds" ///<Malla player por defecto
#define PLAYER_SIENDO_CURADO_MESH "player-plus-verde.3ds" ///<Malla player cuando esta siendo curado


/*para poder obtener datos del player*/
extern vector<CPlayer*>	Players;		///<Vector de jugadores (trabajamos con 1)
extern int				CurrentPlayer;	///<Indice del jugador actual

/** \typedef SUPPORT_STATE
*	Estados que puede tomar el Support durante el juego*/
typedef enum {
	SUPPORT_VIVO,              ///<Señala que el estado del support es vivo
	SUPPORT_MURIENDO,          ///<Señala que el estado del support es muriendo
	SUPPORT_MUERTO,            ///<Señala que el estado del support es muerto
	SUPPORT_DAÑADO,            ///<Señala que el estado del support es dañado
	SUPPORT_CONGELADO,         ///<Señala que el estado del support es congelado
	SUPPORT_CONGELADO_Y_DAÑADO ///<Señala que el estado del support es congelado-dañado
} SUPPORT_STATE;

/** \typedef SUPPORT_COLOR
*	Colores predefinidos para el support (hay una malla distinta para cada estado
*   con estos colores).
*/
typedef enum {
	SUPPORT_COLOR_AMARILLO,  ///<Señala que el color del support es amarillo y las mallas de sus estados seran de este color
	SUPPORT_COLOR_GRIS,      ///<Señala que el color del support es gris y las mallas de sus estados seran de este color
	SUPPORT_COLOR_ROJO       ///<Señala que el color del support es rojo y las mallas de sus estados seran de este color
} SUPPORT_COLOR;

/** \typedef ESTADO_PLAYER
*	Estados que puede tomar el player en referencia a si esta siendo
*   curado o no*/
typedef enum {
	PLAYER_NORMAL,          ///<Señala que el player no esta siendo curado  colocar malla normal
	PLAYER_SIENDO_CURADO    ///<Señala que el player esta siendo curado  colocar malla siendo curado
} ESTADO_PLAYER;

/** \typedef SUPPORT_TIMERS
*	Timers del support*/
typedef enum {
	PERIODO_ENTRE_LAS_CURACIONES,       ///<Controla el tiempo que debe pasar hasta poder volver a curar al player
	PERIODO_CONGELACION,                ///<Controla el tiempo que el support queda congelado tras recibir un disparo cuando no tiene escudo
	PERIODO_PONER_MESH_NORMAL_PLAYER,   ///<Controla el tiempo que pasa el player con la maya que señala que esta siendo curado
	NUMERO_DE_TIMERS_SUPPORT            ///<Sirve para saber el numero de timers a inicializar
} SUPPORT_TIMERS;

/** \typedef SUPPORT_SOUNDS
*	Sonidos que puede disparar el support
*/
typedef enum {
	SUPPORT_SONIDO_REAPARECER,
	SUPPORT_SONIDO_CURAR_PLAYER,
	SUPPORT_SONIDO_MURIENDO,
	SUPPORT_SONIDO_ESCUDO_ROTO,
	SUPPORT_SONIDO_DISPARO_A_ESCUDO,
	SUPPORT_NUM_SONIDOS
} SUPPORT_SOUNDS;

class CSupport : public CExplosiveChar
{
	//Attributes
public:
	//ATRIBUTOS AÑADIDOS
	float		altura;  ///<altura a la que se colocara el support del player
	float		escudo;  ///<Escudo que tendra el support (mientras tiene escudo, los disparos no le paralizan)
	short int	frozen_time_after_impact; ///<Tiempo en ms que quedara paralizado tras recibir un disparo
	float		player_curation; ///<Cantidad de salud que puede curar al player en cada curacion
	float		tiempo_entre_curaciones; ///<Tiempo de espera en ms entre las curaciones al player
	int			num_curaciones; ///<Numero de curaciones que puede realizar el support al player en cada vida del support
	float       umbral_pasa_a_dañado; ///<Umbral de vida del Support a partir del cual se considera que esta danyado
	float       umbral_daño_player; ///<Umbral de vida del Player a partir del cual se le cura

	//ATRIBUTOS COPIADOS DEL PLAYER
	short int	Lives;		///<cantidad de vidas

	//ATRIBUTOS QUE USA SPACE INVADERS POR VERSIONES ANTERIORES PERO NO IMPORTAN
	//bool			Passing2D23D;	// Flag that prevents any destruction of the player, lasers,... while moving from 2D rendering mode to 3D one
	//bool			DiscreteSimulation;

	//ATRIBUTOS INFORMACION
	bool mostrarMensajeParser;          ///<si se pone a true salta un mensaje con los atributos del support al inicio y final del inicialization y level parser
	bool mostrarMensajeDisparoRecibido; ///<si se pone a true, se muestra un mensaje con cada disparo recibido

	//ATRIBUTOS PARTE 3
	SUPPORT_STATE estado; ///<Enumeracion con los estados del support
	SUPPORT_COLOR color;  ///<Enumeracion con los colores predefinidos de support

	
	//Methods

	/**Inicializa los valores por defecto del Support. \n Es usado cuando se crea un Support.\n
	* Ademas de dar valores a los atributos del Support, fija sus timers locales , fija sus alarmas y los activa.*/
	bool Init();	

	/** Es el constructor de la clase CSupport, crea un nuevo support y llama a Init() para inicializar sus atributos */
	CSupport();		

	/**Muestra un mensaje con el valor de los atributos del Support. \n
	*La llamada a este metodo solo funcionara si el atributo CSupport::mostrarMensajeParser es true.\n
	* Cuando CSupport::mostrarMensajeParser sea true, obtendremos 4 mensajes siempre, 2 al principio y final del initialization Parser \n
	* y dos al principio y final del level Parser, esto tiene gran utilidad para comprobar el parseado de los htmls*/
	void muestraAtributos(string tituloVentana);

	/**Muestra un mensaje con el estado, la salud y el escudo del Support asi como la salud del shoot recibido. \n
	*Se mostraran los datos de todos los shoots recibidos cuando el atributo CSupport::mostrarMensajeDisparoRecibido sea true */
	void muestraEstadoTrasImpacto(float saludShoot);

	/**En esta version de Space Invaders, solo funciona el render 3D ,\n
	* por esto, solo esta implementado este modo, en este modo, se toman los valores \n
	* actuales de posicion, rotacion y escala , se aplican a la mesh y se dibuja*/
	void Render();

	/**Mueve el support en caso de que sea necesario, cambia la malla del player si es necesario \n
	* (si ya termino de curarlo) y llama a runLogic()
	*/
	void Update();

	/**Algunas de las acciones que se realizan aqui son : comprobar si se recibe daño ,cambiar el estado y la malla \n
	*comprobar si algun timer suena, curar al player si es necesario ...*/
	void runLogic();

	/**Tras recibir el estado al que ha pasado el Support, si este tiene un color predefinido \n
	* (atributo CSupport::color) (etiqueta levelParser SUPPORT_COLOR) se obtiene el nombre de la mesh \n
	* de ese estado y ese color y se cambia, sino tiene color predefinido, se toma una mesh cualquiera*/
	void cambiaMesh(SUPPORT_STATE estado);

	/**Este metodo realiza la deteccion y el tratamiento de colisiones \n
	* esta implementado, y se busca que tanto los disparos enemigos como los del\n
	* player tengan efecto en el Support,pero en esta versison de SI no funciona,\n
	* por lo que hacemos una aproximacion a la deteccion de colisiones en el metodo disparo_recibido()*/
	void Collided(CCharacter *CollidedChar);

	/**Cuando se le pasa el estado del player, le cambia la mesh: \n
	*Si lo esta curando le pone una mesh con un plus verde\n
	*Sino le pone su mesh por defecto (este caso se da cuando se suena el timer PERIODO_PONER_MESH_NORMAL_PLAYER).*/
	void cambiaMeshPlayer(ESTADO_PLAYER e);

	/**Como en esta version de SI la deteccion de colisiones no funciona, \n
	* obtenemos la lista de shoots activos (ShootsManager->ShootsAct) y comprobamos si \n
	* algun shoot colisiona obteniendo su posicion y si es el caso, devolvemos el daño recibido.*/
	float disparo_recibido(void);

	/**Este metodo es llamado desde runLogic() cuando un Support esta en el \n
	* estado SUPPORT_MUERTO pero el atributo CSupport::Lives es mayor que 1, \n
	* el metodo asigna a los atributos que han sido modificados desde el level parser \n
	* (posicion, rotacion, escala, escudo, vida etc) los valores leidos en el levelParser\n
	* que tiene Support_tras_level_parser*/
	void reinicia_los_valores_level_parser(void);
};


#endif
