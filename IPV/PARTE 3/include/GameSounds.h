/**
*	GameSounds.h
*	Definition of the class Game Sounds
*	General class to support all the different types of sounds listened during the playing game
*	Prefix: CSIG_

*	@author Ramon Molla
*	@version 2011-11
*/

#ifndef CSIG_CLASS_GAME_SOUNDS
#define CSIG_CLASS_GAME_SOUNDS

#include <UGKString.h>

/**   
   * It defines the types of sounds for the game
   * @param enum CSIG_SOUNDS: which lists the types of sounds for the game
*/

//#define CGS_SOUND_TAG_MAX_LONG 32
#define CSG_MAX_MUSIC_TRACKS 4

typedef enum
{
	CGS_NO_SND,	///<There is no sound played
	CGS_APPLAUSE_SND,	///<Player has won a level and an applause is listened
	CGS_BONUS_SND,		///<A bonus is got and an improvement is got
	CGS_CROSS_SND,		///<Passing from 2D to 3D loop 
	CGS_DESCEND_SND,	///<Ship descends down one step
	CGS_EXPLOSION_SND,	///<Explosion Sound
	CGS_KEY_SND,		///<Touched
	CGS_SHIELD_SND,		///<Passing from 2D to 3D loop 
	CGS_SHOOT_SND,		///<Player's shoot sound
	CGS_SHOOT3D_SND,	///<Player's shoot sound when in 3D mode
	CGS_TOUCH_SND,		///<Shoot sound when a hit is got
	CGS_SUPPORT_SONIDO_REAPARECER,
	CGS_SUPPORT_SONIDO_MURIENDO,
	CGS_SUPPORT_SONIDO_ESCUDO_ROTO,
	CGS_SUPPORT_SONIDO_DISPARO_A_ESCUDO,
	CGS_SUPPORT_SONIDO_CURAR_PLAYER,
	CGS_MAX_SND	///<For accounting purpouses only
} CSIG_SOUNDS;

extern UGKS_String CGS_NameSounds	[CGS_MAX_SND];
extern UGKS_String CGS_MusicFiles	[CSG_MAX_MUSIC_TRACKS];

#endif
