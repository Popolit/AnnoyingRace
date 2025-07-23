#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DrawCharacterWidget.generated.h"

DECLARE_DELEGATE(FOnDrawAnimationFinished)

/**
 * Draw Character Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UDrawCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWidget(const class UCharacterData* _CharacterData);

	void ShowWidget();

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation) override;

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_CharacterName_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> Img_Character_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> Txt_SkillInfo_;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		TObjectPtr<UWidgetAnimation> Anim_FadeInOut_;

public:
	FOnDrawAnimationFinished OnDrawAnimationFinished_;
};
