#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaceRankSlotWidget.generated.h"

/**
 * Race Ranking Slot
 */
UCLASS()
class ANNOYINGRACE_API URaceRankSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime) override;

public:
	void SetRank(uint8 _Rank);

	void SetImage(TSoftObjectPtr<UTexture2D> _Image);

	void SetText(const FString& _Text);

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> Img_UserCharacter_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_UserID_;

	float TargetYPosition_;
	float CurrentYPosition_;
};
