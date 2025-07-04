#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DrawCharacterWidget.generated.h"

/**
 * Draw Character Widget
 */
UCLASS()
class ANNOYINGRACE_API UDrawCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> TxtCharacterName_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> ImgCharacter_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> TxtSkillInfo_;
};
