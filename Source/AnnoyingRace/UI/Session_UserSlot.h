#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Session_UserSlot.generated.h"

/**
 * User Slot Widget for Session
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API USession_UserSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSlot(const struct FSessionPlayerInfo& _PlayerInfo);

private:
	void SetEmptySlot();

	void SetValidSlot();
	
private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UImage> Img_Host_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UImage> Img_Ready_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_UserName_;
};
