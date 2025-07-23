#include "DrawCharacterWidget.h"

#include "Characters/CharacterData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UDrawCharacterWidget::SetWidget(const UCharacterData* _CharacterData)
{
	Txt_CharacterName_->SetText(_CharacterData->GetCharacterName());
	Img_Character_->SetBrushFromSoftTexture(_CharacterData->GetCharacterIcon());
	Txt_SkillInfo_->SetText(_CharacterData->GetSkillInfo());
}

void UDrawCharacterWidget::ShowWidget()
{
	SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(Anim_FadeInOut_);
}

void UDrawCharacterWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation)
{
	if(_Animation == Anim_FadeInOut_)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		OnDrawAnimationFinished_.ExecuteIfBound();
	}
}
