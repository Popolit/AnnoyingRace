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

private:
	TWeakObjectPtr<class UCommonSession_SearchResult> SessionSearchResult_;
	FString SessionPassword_;
};
