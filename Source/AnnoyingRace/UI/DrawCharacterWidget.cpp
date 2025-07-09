#include "DrawCharacterWidget.h"

#include "Characters/CharacterData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UDrawCharacterWidget::SetWidget(const UCharacterData* _CharacterData)
{
	TxtCharacterName_->SetText(_CharacterData->GetCharacterName());
	ImgCharacter_->SetBrushFromSoftTexture(_CharacterData->GetCharacterIcon());
	TxtSkillInfo_->SetText(_CharacterData->GetSkillInfo());
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
		SetVisibility(ESlateVisibility::Hidden);
		OnDrawAnimationFinished_.ExecuteIfBound();
	}
}
