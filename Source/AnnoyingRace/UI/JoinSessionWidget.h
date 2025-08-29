#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoinSessionWidget.generated.h"

/**
 * Join Session Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UJoinSessionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void OnMapChanged(FString _MapName, ESelectInfo::Type _Type);

	UFUNCTION()
		void OnMinUserChanged(const FText& _Text);
	
	UFUNCTION()
		void OnMinUserCommitted(const FText& _Text, ETextCommit::Type _CommitMethod);

	UFUNCTION()
		void OnMaxUserChanged(const FText& _Text);
	
	UFUNCTION()
		void OnMaxUserCommitted(const FText& _Text, ETextCommit::Type _CommitMethod);

	UFUNCTION()
		void OnClickRefresh();

private:
	void OnSessionFindFinished(bool _bSuccess, const FText& _ErrorMessage, const TArray<class UCommonSession_SearchResult*>& _Results);

	FPrimaryAssetId FindMapAssetIdByMapName(const FString& _MapName);
	
private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UScrollBox> SCB_SessionList_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UComboBoxString> CBB_MapList_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> Img_Map_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UEditableTextBox> ETB_SessionName_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UEditableTextBox> ETB_MinUserCount_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UEditableTextBox> ETB_MaxUserCount_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_Back_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Refresh_;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class USessionSlotWidget> SessionSlotWidgetClass_;

private:
	FText LastValidMinUserCount_;
	FText LastValidMaxUserCount_;
};
