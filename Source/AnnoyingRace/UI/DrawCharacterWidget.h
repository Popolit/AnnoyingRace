#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DrawCharacterWidget.generated.h"

DECLARE_DELEGATE(FOnDrawAnimationFinished)

/**
 * Draw Character Widget
 */
UCLASS()
class ANNOYINGRACE_API UDrawCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWidget(const class UCharacterData* _CharacterData);

	void ShowWidget();

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation) override;

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> TxtCharacterName_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> ImgCharacter_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> TxtSkillInfo_;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		TObjectPtr<UWidgetAnimation> Anim_FadeInOut_;

public:
	FOnDrawAnimationFinished OnDrawAnimationFinished_;
};
