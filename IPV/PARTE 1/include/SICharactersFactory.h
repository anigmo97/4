//FICHERO MODIFICADO-E1
/**	
#file SICharactersFactory.h
Definition of the class S.I. Characters Factory

	Prefix: CSIF_

	@author Ramon Molla
	@version 2016-05
*/

#ifndef CSIF_CHARACTER_FACTORY
#define CSIF_CHARACTER_FACTORY

#include <UGKCharactersFactory.h>
#include <GameCharacters.h>

using namespace UGK;

class CSICharactersFactory : public CCharactersFactory
{
	
public:
	CSICharactersFactory() {}

	/**
	* @brief CSICharactersFactory \n
	* Represents a Factory of CCharacters objects. Implements CCharactersFactory.
	*
	* @fn     createNewCharacter
	* @param  int characterType Type of CCharacter to be created.
	* @return Pointer to the new CCharacter created.
	* @brief  Creates a new CCharacter of the specified type.
	*/
	CCharacter* createNewCharacter(int characterType, int subType);


	/**
	* @fn     getNumDistinctCharacterTypes
	* @return The number of different types of Characters that this factory can produce.
	* @brief  Gets the total number of different types of Characters that this factory can produce.
	*/
	int getNumDistinctCharacterTypes();

};

#endif