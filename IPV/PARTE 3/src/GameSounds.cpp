//MODIFICADO-E3
/**The place where resides the class GameSounds
*  General class to support all the different sounds of the game

*	@author Carlos Martinez
*	@version 2012
*/
#include <GameSounds.h>

UGKS_String CGS_NameSounds[CGS_MAX_SND] = 
{
	"CGS_NO_SOUND",		///<There is no sound played
	"CGS_APPLAUSE",		///<Player has won a level and an applause is listened
	"CGS_BONUS",		///<A bonus is got and an improvement is got
	"CGS_CROSS",		///<Passing from 2D to 3D loop 
	"CGS_DESCEND",		///<Ship descends down one step
	"CGS_EXPLOSION",	///< Explosion Sound
	"CGS_KEY",			///<Touched	
	"CGS_SHIELD",		///<Passing from 2D to 3D loop 
	"CGS_SHOOT",		///<Player's shoot sound
	"CGS_SHOOT3D",		///<Player's shoot sound when in 3D mode
	"CGS_TOUCH"			///<Shoot sound when a hit is got
	"CGS_SUPPORT_SONIDO_REAPARECER",
	"CGS_SUPPORT_SONIDO_CURAR_PLAYER",
	"CGS_SUPPORT_SONIDO_MURIENDO",
	"CGS_SUPPORT_SONIDO_ESCUDO_ROTO",
	"CGS_SUPPORT_SONIDO_DISPARO_A_ESCUDO"
};


////Se deberia de hacer por fichero de inicializacion o por parser de nivel
//UGKS_String CGS_SoundsFiles[CGS_MAX_SND] = 
//{
//	"nosound.mp3",		///<There is no sound played
//	"applause.mp3",		///<Player has won a level and an applause is listened
//	"bonus.mp3",		///<A bonus is got and an improvement is got
//	"cross.mp3",		///<Passing from 2D to 3D loop 
//	"descent.mp3",		///<Ship descends down one step
//	"explosion.mp3",	///<Explosion Sound
//	"key.mp3",			///<Touched
//	"shield_charge.mp3",///<Passing from 2D to 3D loop 
//	"shoot.mp3",		///<Player's shoot sound
//	"shoot_3d.mp3",		///<Player's shoot sound when in 3D mode
//	"touch.mp3"			///<Shoot sound when a hit is got
//};

// The SDL Chunk structure doesn't support mp3 files
//Se deberia de hacer por fichero de inicializacion o por parser de nivel
//UGKS_String CGS_SoundsFiles[CGS_MAX_SND] = 
//{
//	"nosound.ogg",		///<There is no sound played
//	"applause.ogg",		///<Player has won a level and an applause is listened
//	"bonus.ogg",		///<A bonus is got and an improvement is got
//	"cross.ogg",		///<Passing from 2D to 3D loop 
//	"descent.ogg",		///<Ship descends down one step
//	"explosion.ogg",	///<Explosion Sound
//	"key.ogg",			///<Touched
//	"shield_charge.ogg",///<Passing from 2D to 3D loop 
//	"shoot.ogg",		///<Player's shoot sound
//	"shoot_3d.ogg",		///<Player's shoot sound when in 3D mode
//	"touch.ogg"			///<Shoot sound when a hit is got
//};

UGKS_String CGS_SoundsFiles[CGS_MAX_SND] = 
{
	"nosound.wav",		///<There is no sound played
	"applause.wav",		///<Player has won a level and an applause is listened
	"bonus.wav",		///<A bonus is got and an improvement is got
	"cross.wav",		///<Passing from 2D to 3D loop 
	"descent.wav",		///<Ship descends down one step
	"explosion.wav",	///<Explosion Sound
	"key.wav",			///<Touched
	"shield_charge.wav",///<Passing from 2D to 3D loop 
	"shoot.wav",		///<Player's shoot sound
	"shoot_3d.wav",		///<Player's shoot sound when in 3D mode
	"touch.wav",			///<Shoot sound when a hit is got
	"support_reaparecer.wav",
	"support_muriendo.wav",
	"support_escudo_roto.wav",
	"support_disparo_escudo.wav",
	"support_curar_player.flac"
};


UGKS_String CGS_MusicFiles[CSG_MAX_MUSIC_TRACKS] = 
{
	"music_harder_better_faster.ogg",
	"music_around_the_world.ogg",
	"music_one_more_thing.ogg",
	"1_music_jt_strng.ogg"
};