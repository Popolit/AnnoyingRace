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
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime) override;

public:
	void SetPlayerName(const FString& _PlayerName) const;
	
	void SetSlotFromCharacterData(const class UCharacterData* _CharacterData);

	void SetRank(uint8 _Rank);

private:
	void SetImage(TSoftObjectPtr<UTexture2D> _Image) const;

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> Img_UserCharacter_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_PlayerName_;

	float TargetYPosition_;
	float CurrentYPosition_;
	float SizeY_;
};
