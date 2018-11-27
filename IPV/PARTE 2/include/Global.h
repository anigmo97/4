//FICHERO MODIFICADO-E1
/**	Global definitions, types, includes and so on

*	Prefix: SIGLB_

*	@author Ramon Molla
*	@version 2013-06
*/


#ifndef SIGLB_GLOBAL_H
#define SIGLB_GLOBAL_H

#include <GUI.h>

//Loading managers
#include <AI/AIManager.h>
#include <BonusManager.h>	// Header File class Bonus Manager
#include <FontsManager.h>
#include <UGKMeshManager.h>
#include <UGKMusicsManager.h>
#include <ScoreManager.h>
#include <ShootsManager.h>	/// Header File class Manager for the shoots
#include <UGKTexturesManager.h>/// Header File class Manager for the Textures.
#include <TextureAnimationManager.h>
#include <SIMessage.h>
#include <SICharactersFactory.h>
//AÑADIDO PARTE ENTREGA1
#include "Support.h"
#include <Support.h>

#include <Application.h>

#include <UGKCameraManager.h>	// Header File class for the Cameras Manager

#include <SISceneGraph.h>	
#include <Background.h>

//HW
#include <SIKeyboard.h>
#include <UGKGraphicsCard.h>
#include <UGKAudioHardware.h>
#include <UGKCamera.h>

//#define CHW_USE_KINECT
#ifdef CHW_USE_KINECT
#include <UGKKinectManager.h>
#endif

#include <UGKLog.h>

using namespace UGK;

/**
	* Define all variables and global functions of the game
	* Equalizer, Sound, Windows, Character (Nav), Bonus, enmity, Time, 3D, 2D
*/

///General game singletons
extern CApplication			Application;
extern CBackground			Background;
extern CCamera				*Camera;			///<Singleton declaration of the game camera

extern CSICharactersFactory	*CharacterFactory;	///<Singleton declaration of the character factory that creates as many given characters as demanded
extern CMsgFactory			MsgFactory;

extern CCharactersPool		*CharacterPool;   ///< Pool of CCharacters

//AÑADIDO PARTE ENTREGA1
extern CSupport* defaultSupport;

//RT-DESK
extern RTDESK_CEngine		*RTDESK_Engine;

extern CGUI					GUI;				///<Singleton declaration to render the general configuration of the game interface
extern CLog					Log;				///<For logging performance, tracing,...

/**
*	\defgroup GSingletons HW game singletons 
*
*	@{
*/
extern CSIKeyboard		SIKeyboard;		///<Singleton to manage the application keyboard
extern CGraphicCard		GraphicsCard;	///<Singleton to manage the application Graphics Card

extern CAIManager		AIManager;
extern CBonusManager	*BonusManager;
extern CFontsManager	FontsManager;
extern CMeshesManager	MeshesManager;
extern CMusicsManager	MusicsManager;
extern CScoresManager	ScoresManager;
extern CShootsManager	*ShootsManager;
extern CSoundsManager	SoundsManager;
extern CTextureAnimationManager	AnimationsManager;
extern CTexturesManager	TexturesManager;
extern SISceneGraph		SceneGraph;

#ifdef CHW_USE_KINECT
extern CKinectManager	KinectManager;
#endif
#ifdef CHW_USE_WIIMOTE
extern CWiimoteManager	WiimotesManager;
#endif

extern float			UGKALG_NullMatrix[16];

#endif
