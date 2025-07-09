#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h" 
#include "CharacterData.generated.h"

/**
 * Character Data Asset
 */

class APlayableCharacter;

UCLASS(Blueprintable)
class ANNOYINGRACE_API UCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<APlayableCharacter> GetCharacterClass() const;

	FText GetCharacterName() const;

	TSoftObjectPtr<UTexture2D> GetCharacterIcon() const;

	FText GetSkillInfo() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<APlayableCharacter> CharacterClass_;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText CharacterName_;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSoftObjectPtr<UTexture2D> CharacterIcon_;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText SkillInfo_;
};	
