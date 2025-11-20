#include "SkillWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SkillComponent.h"

void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->GetOnNewPawnNotifier().AddUObject(this, &USkillWidget::OnPawnChanged);
		OnPawnChanged(GetOwningPlayerPawn());
	}
	
	DMI_SkillCoolDown_ = Img_SkillCoolDown_->GetDynamicMaterial();
	Img_SkillCoolDown_->SetBrushFromMaterial(DMI_SkillCoolDown_);
}

void USkillWidget::NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime)
{
	Super::NativeTick(_MyGeometry , _InDeltaTime);
	
	if (nullptr == CachedSkillComp_)
	{
		return;
	}
	
	int32 SkillCount = CachedSkillComp_->GetSkillCount();
	if (0 == SkillCount)
	{
		Txt_SkillCount_->SetText(FText());
	}
	else if (-1 == SkillCount)
	{
		Txt_SkillCount_->SetText(FText::FromString("INF"));
	}
	else
	{
		Txt_SkillCount_->SetText(FText::AsNumber(CachedSkillComp_->GetSkillCount()));
	}
	float SkillCoolDownRatio = CachedSkillComp_->GetSkillCoolDownRatio();

	DMI_SkillCoolDown_->SetScalarParameterValue("CoolDownPercent", 1.f - SkillCoolDownRatio);
}

void USkillWidget::OnPawnChanged(APawn* _NewPawn)
{
	if (_NewPawn)
	{
		CachedSkillComp_ = _NewPawn->GetComponentByClass<USkillComponent>();

		if (nullptr == CachedSkillComp_)
		{
			SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
		Img_SkillIcon_->SetBrushFromSoftTexture(CachedSkillComp_->GetSkillImg());
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
