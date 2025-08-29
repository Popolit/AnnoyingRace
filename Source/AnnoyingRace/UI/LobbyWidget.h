#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * Lobby Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_CreateRoom_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_JoinGame_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Option_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Credits_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Exit_;
};
