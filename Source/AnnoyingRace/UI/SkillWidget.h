#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

/**
 * Skill Widget (Icon)
 */
UCLASS()
class ANNOYINGRACE_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime) override;

private:
	void OnPawnChanged(APawn* _NewPawn);
private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> Img_SkillIcon_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UImage> Img_SkillCoolDown_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_SkillCount_;

	UPROPERTY()
		TObjectPtr<UMaterialInstanceDynamic> DMI_SkillCoolDown_;

private:
	TObjectPtr<class USkillComponent> CachedSkillComp_;
};
