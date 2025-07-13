#include "Buff_Speed.h"

#include "Components/StatusComponent.h"


UBuff_Speed::UBuff_Speed()
{
	Amount_ = 0.f;
}

void UBuff_Speed::Buff(AActor* _OwnerActor)
{
	auto StatusComponent = Cast<UStatusComponent>(_OwnerActor->GetComponentByClass(UStatusComponent::StaticClass()));

	if(ensureMsgf(StatusComponent, TEXT("Buff Owner doesn't have Status Component")))
	{
		if(bMultiply)
		{
			StatusComponent->MultiplySpeedBonus(Amount_);
		}
		else
		{
			StatusComponent->AddSpeedBonus(Amount_);
		}
	}
}

void UBuff_Speed::OnBuffEnd(AActor* _OwnerActor)
{
	auto StatusComponent = Cast<UStatusComponent>(_OwnerActor->GetComponentByClass(UStatusComponent::StaticClass()));

	if (ensureMsgf(StatusComponent, TEXT("Buff Owner doesn't have Status Component")))
	{
		if (bMultiply)
		{
			StatusComponent->MultiplySpeedBonus(-Amount_);
		}
		else
		{
			StatusComponent->AddSpeedBonus(-Amount_);
		}
	}
}
