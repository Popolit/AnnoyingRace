#include "CharacterData.h"

TSubclassOf<APlayableCharacter> UCharacterData::GetCharacterClass() const
{
	return CharacterClass_;
}

FText UCharacterData::GetCharacterName() const
{
	return CharacterName_;
}

TSoftObjectPtr<UTexture2D> UCharacterData::GetCharacterIcon() const
{
	return CharacterIcon_;
}

FText UCharacterData::GetSkillInfo() const
{
	return SkillInfo_;
}
