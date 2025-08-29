#include "Session_UserSlot.h"

#include "SessionGameState.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USession_UserSlot::SetSlot(const FSessionPlayerInfo& _PlayerInfo)
{
	//빈 슬롯일 경우
	if (false == _PlayerInfo.UniqueNetId_.IsValid())
	{
		SetEmptySlot();
		return;
	}

	
	SetValidSlot();
	//호스트일 경우
	if (_PlayerInfo.bIsHost_)
	{
		Img_Host_->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Img_Host_->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	Img_UserProfile_->SetBrushFromSoftTexture(_PlayerInfo.PlayerImg_);
	if (_PlayerInfo.bIsReady_)
	{
		Img_Ready_->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Img_Ready_->SetVisibility(ESlateVisibility::Collapsed);
	}
	Txt_UserName_->SetText(FText::FromString(_PlayerInfo.PlayerName_));
}

//빈 슬롯
void USession_UserSlot::SetEmptySlot()
{
	Img_UserProfile_->SetVisibility(ESlateVisibility::Collapsed);
	Img_Host_->SetVisibility(ESlateVisibility::Collapsed);
	Txt_UserName_->SetVisibility(ESlateVisibility::Collapsed);
	Img_Ready_->SetVisibility(ESlateVisibility::Collapsed);
}

//유저가 있는 슬롯 Visible은 강퇴 등 마우스 상호작용을 위해
void USession_UserSlot::SetValidSlot()
{
	Img_UserProfile_->SetVisibility(ESlateVisibility::Visible);
	Txt_UserName_->SetVisibility(ESlateVisibility::Visible);
}
