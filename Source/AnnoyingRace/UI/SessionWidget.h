#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionWidget.generated.h"

/**
 * Session Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API USessionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation) override;

public:
	void AddChatOnLog(const FText& _Chat, const FString& _PlayerName);
	
	void BeginCountDown();

	void StopCountDown();
	
private:
	void UpdateSessionInfo(const struct FSessionInfo& _SessionInfo);

	void UpdatePlayerList(const TArray<struct FSessionPlayerInfo>& _PlayerList);


private:
	void UpdateUIForHost();

	UFUNCTION()
		void OnMapChanged(FString _SelectedMap, ESelectInfo::Type _SelectionType);
	
	UFUNCTION()
		void OnClickedReadyBtn();

	UFUNCTION()
		void OnClickedBackBtn();

	UFUNCTION()
		void OnSessionNameChanged(const FText& _Text, ETextCommit::Type _CommitMethod);

	UFUNCTION()
		void OnPasswordChanged(const FText& _Text, ETextCommit::Type _CommitMethod);

	UFUNCTION()
		void OnPrivateStateChanged(bool _bChecked);

	UFUNCTION()
		void OnPlayerChatted(const FText& _Text, ETextCommit::Type _CommitMethod);

	UFUNCTION(BlueprintCallable)
		void UpdateCountDown();
	
private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UVerticalBox> VB_UserList_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UComboBoxString> CBB_MapList_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> Img_Map_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_Ready_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Back_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UEditableText> EDT_SessionName_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UEditableTextBox> ETB_Password_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UCheckBox> CHB_IsPrivate_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UScrollBox> SCB_ChatLog_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UEditableTextBox> ETB_Chat_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_CountDown_;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		TObjectPtr<UWidgetAnimation> Anim_CountDown_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class USession_UserSlot> SessionUserSlotClass_;

private:
	uint8 Count_;
};
