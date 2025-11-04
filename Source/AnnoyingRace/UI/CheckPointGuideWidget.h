#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheckPointGuideWidget.generated.h"

/**
 * CheckPoint Guide Widget
 * 체크 포인트까지의 거리 및 방향을 안내하는 위젯
 */
UCLASS()
class ANNOYINGRACE_API UCheckPointGuideWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> Img_GuideMarker_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_Distance_;

	UPROPERTY(EditDefaultsOnly)
		float Padding_ = 50.f;

private:
	TObjectPtr<APlayerController> OwnerController_;
	TObjectPtr<class ARacePlayerState> PlayerState_;
};
