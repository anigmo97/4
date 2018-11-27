/**	Class that loads the values by default in the very beginning of the game

*	Prefix CIP_: Class Initialization Parser

*	@author Ramon Molla
*	@version 2007-09-11
*/

#ifndef CIP_INITIALIZATION_H
#define CIP_INITIALIZATION_H

#include <UGKHTMLParser.h>
#include <GameCharacters.h>
#ifndef FSTREAM_INITIALIZED
#define FSTREAM_INITIALIZED
#include <fstream>
#endif

/** \typedef CIP_TagType

*	Types of tags the initialization parser can match during the parsing
*	They can be used also as states of of the stack machine (parser) during the parsing
*/
typedef enum {
	ACCELERATION = MAXTAGS_D,
	AI,
	ALTURA, //AÑADIDO ENTREGA1
	ANIMATION2D,
	CENTER,
	CHARACTER,				///<default values for a given character of the game
	COLLIDED,
	COLLISION,
	COLLISION_TABLE,
	CONFIG,
	DIR,					///<Specifies the folders where to find resources: images, languages,...
	EPS,					///<Energy Per Shoot
	ESCUDO, //AÑADIDO PARTE ENTREGA1
	EXPLOSION,
	FILE_IP,
	FROZEN_TIME_AFTER_IMPACT, //AÑADIDO PARTE ENTREGA1
	GEOMETRY,				///<Folder where to find the music and sounds
	HAVE,					///<Recursive definition in a character. A character that has mor characters inside. Not inherited but held
	HEALTH,					///<How strong is the character. How much has to be hurt before dieing
	LANGUAGE,				///<Folder where to find the languages for the interface
	LEVEL,
	LINE,
	LIVES,					///<Amount of lives the Player has still before finishing the game
	MESH,					///<3DS geometry associated to a given character //V Eliminar y cambiar por MESH_CHARACTER
	MUSIC,					///<Folder where to find the music and sounds
	NAME,
	PERIODE_ANI2D_CHAR,
	PLAYER_CURATION, //AÑADIDO PARTE ENTREGA1
	POSITION_IP,
	RADIO,
	ROTATION_IP,
	SCALE_IP,
	SCORE,
	SCORES,
	SHOOT,					///<Kind of shooting
	SPEED,					///<Character speed
	SPS,					///<Shoots Per Second
	SOUNDS,
	TEXTURE,
	TEXTURE2D,
	TEXTURE3D,
	VELOCITY,
	X_IP,						///<First of the three space coordinates that may be used for coding the speed, position,...
	Y_IP,						///<Second of the three space coordinates that may be used for coding the speed, position,...
	Z_IP,						///<Third of the three space coordinates that may be used for coding the speed, position,...
	MAXTAGS_IP					///<This tag is for management purpouses only. There is no correspondence to any string label in the HTML file
} CIP_TagType;

typedef enum {
	CIP_MIN_LIVES_NOT_DEF,
	CIP_MAX_LIVES_NOT_DEF,
	CIP_LOWER_THAN_MIN,
	CIP_HIGHR_THAN_MAX,
	CIP_AMOUNT_ALLOWED,
	CIP_VALUE_CUT,
	CIP_MINIMUM,
	CIP_MAXIMUM,
	CIP_INITIALIZATION,
	CIP_SPEED_N,
	CIP_ERRONEUS,
	CIP_TAG,
	CIP_NOT_ALLOWED,
	CIP_MAX_MSGS
} CIP_LogMsg;

extern UGKS_String CIP_Tags[MAXTAGS_IP - MAXTAGS_D];

/**	
*	CInitializationParser

*	@author Ramon Molla
*	@version 2007-09-11
*/
using namespace UGK;

class CInitializationParser : public CHTMLParser
{
private:

	LexemeDictionaryType CharNameId;

	void BeginParse(DWORD dwAppData, bool &bAbort);
    void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
    void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
    void Characters(const UGKS_String &rText, DWORD dwAppData, bool &bAbort);
    void Comment(const UGKS_String &rComment, DWORD dwAppData, bool &bAbort);
    void EndParse(DWORD dwAppData, bool bIsAborted);

	//Methods for changing internal values of default/generic objects
	void ChangeDimValue	(SpaceCoords Dim, double Value);

	/**
	@fn void ChangeSpeed(CCharacter *Character, SpaceCoords Dim, double Value)
	@param CCharacter *Character : Character where to apply the new value
	@param SpaceCoords Dim	: Dimension where to apply the change
	@param double Value		: New value to apply
	Depending on the character being updated the dimension Dim of the speed is changed by the new Value
	*/
	inline void ChangeSpeed(CCharacter *Character, SpaceCoords Dim, UGKALG_NUMERIC_TYPE Value)	{ Character->Speed.v[Dim] = Value; }
	void		ChangeCenter(CCharacter *Character, SpaceCoords Dim, UGKALG_NUMERIC_TYPE Value);
	
	/**
	@fn void ChangePosition(CCharacter *Character, SpaceCoords Dim, double Value);
	@param CCharacter *Character : Character where to apply the new value
	@param SpaceCoords Dim	: Dimension where to apply the change
	@param double Value		: New value to apply
	Depending on the character being updated the dimension Dim of the position is changed by the new Value
	*/
	inline void ChangePosition(CCharacter *Character, SpaceCoords Dim, UGKALG_NUMERIC_TYPE Value)	{ Character->MoveTo(Dim, Value); }

	/**
	@fn void ChangeRotation(CCharacter *Character, SpaceCoords Dim, double Value);
	@param CCharacter *Character : Character where to apply the new value
	@param SpaceCoords Dim	: Dimension where to apply the change
	@param double Value		: New value to apply
	Depending on the character being updated the dimension Dim of the rotation is changed by the new Value
	*/
	inline void ChangeRotation(CCharacter *Character, SpaceCoords Dim, UGKALG_NUMERIC_TYPE Value)	{ Character->Rotation.v[Dim] = Value; }

	/**
	@fn void ChangeScale(CCharacter *Character, SpaceCoords Dim, double Value);
	@param CCharacter *Character : Character where to apply the new value
	@param SpaceCoords Dim	: Dimension where to apply the change
	@param double Value		: New value to apply
	Depending on the character being updated the dimension Dim of the scale is changed by the new Value
	*/
	inline void ChangeScale(CCharacter *Character, SpaceCoords Dim, UGKALG_NUMERIC_TYPE Value)	{ Character->Scale.v[Dim] = Value; }

	/**
	@fn void void InitializeDefaults()
	Initialize default parameters for every default character
	*/
	void InitializeDefaults();
	void InitializeSounds2DefaultChar();

	void AssignTMG(CCharacter *Character);

	/**
	*	\fn void AssignTags DHTML_TagsArrayType Tags, DHTML_TagsArray OwnTags, unsigned int Size)
	*	Copy internal Tags for parsing into the external tags of the corresponding parser
	*	\param[out] Tags	The array destination
	*	\param[in]  OwnTags	The array with the tags related to the current parser
	*	\param[in]  Size	Amount of tags counting CP_ParserTags and current parser ones
	*/
	inline void AssignCharacterTags()
	{
		int TagsIndex;

		CharNameId.clear();

		for (TagsIndex = 0; TagsIndex<CHARS_MAX_CHARTYPE; TagsIndex++) CharNameId[CHARS_Tags[TagsIndex]] = TagsIndex;
	}

public:

	void Init(UGKS_String FileName);

	///Constructor
	CInitializationParser();

	///Translates from a string to the token that the string represents
	CP_TokenType CharLexeme2Token(CP_LexemeType Lexeme)
	{
		CurrentToken = CharNameId.find(Lexeme);
		if (CharNameId.end() == CurrentToken)
			return CP_NO_TOKEN;
		else return CurrentToken->second;
	}

	///Given a token, this method converts it into its lexeme (string) associated to it
	CP_LexemeType Token2CharLexeme(CP_TokenType Token)
	{
		for (CurrentToken = CharNameId.begin(); CurrentToken != CharNameId.end(); ++CurrentToken)
			if (Token == CurrentToken->second)
				return  (CP_LexemeType)CurrentToken->first;
		return UGKS_EMPTY_STRING;	//In case no matching happens
	}
};

#endif