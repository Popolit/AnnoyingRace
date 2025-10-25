#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionSlotWidget.generated.h"

/**
 * Session Slot Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API USessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetSessionData(class UCommonSession_SearchResult* _SessionSearchResult);
	
	void SetSessionName(const FString& _SessionName);

	void SetUserCount(int32 _CurrUserCount, int32 _MaxUserCount);

	void SetIsPrivateSession(bool _bPrivate);

	void SetMapThumbnail(const TSoftObjectPtr<UTexture2D>& _MapThumbnail);

private:
	UFUNCTION()
		void OnClickedEnter();
private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> Img_Map_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_SessionName_;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UImage> Img_Lock_;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> Txt_UserCount_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_Enter_;

	TWeakObjectPtr<UCommonSession_SearchResult> SessionSearchResult_;
};
