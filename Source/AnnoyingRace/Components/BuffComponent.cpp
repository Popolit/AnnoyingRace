#include "BuffComponent.h"

#include "Net/UnrealNetwork.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	BuffDuration_ = -1.f;
}

void UBuffComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBuffComponent, BuffRemain_);
}

TSoftObjectPtr<UTexture2D> UBuffComponent::GetBuffImg()
{
	return BuffImg_;
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = GetOwner();
	if(ensureMsgf(Actor, TEXT("Buff Owner was nullptr")))
	{
		BuffRemain_ = BuffDuration_;
		Buff(Actor);
	}
}

void UBuffComponent::TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction)
{
	Super::TickComponent(_DeltaTime, _TickType, _ThisTickFunction);

	AActor* Actor = GetOwner();
	if (ensureMsgf(Actor, TEXT("Buff Owner was nullptr")))
	{
		if(Actor->HasAuthority())
		{
			if (0.f < BuffRemain_)
			{
				BuffRemain_ -= _DeltaTime;
				if (BuffRemain_ <= 0.f)
				{
					OnBuffEnd(Actor);
				}
			}
		}
	}
	
}

void UBuffComponent::OnRep_BuffRemain()
{
	//TODO : 버프 UI 업데이트	
}


