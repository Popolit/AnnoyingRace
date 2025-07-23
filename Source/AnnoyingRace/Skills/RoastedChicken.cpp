#include "RoastedChicken.h"

#include "Characters/PlayableCharacter.h"
#include "Characters/States/State.h"
#include "Components/StateComponent.h"
#include "Components/BuffComponent.h"

ARoastedChicken::ARoastedChicken()
{
	Lifespan_ = 20.f;
	
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MeshComponent_ = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent_->SetupAttachment(GetRootComponent());

	MeshComponent_->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	MeshComponent_->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ARoastedChicken::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Lifespan_);
	OnActorBeginOverlap.AddDynamic(this, &ARoastedChicken::OnCollisionOverlapped);
}

void ARoastedChicken::OnCollisionOverlapped(AActor* _OverlappedActor, AActor* _OtherActor)
{
	if(HasAuthority())
	{
		APlayableCharacter* TargetCharacter = Cast<APlayableCharacter>(_OtherActor);
		if(nullptr == TargetCharacter)
		{
			return;
		}
		auto StateComponent = Cast<UStateComponent>(TargetCharacter->GetComponentByClass(UStateComponent::StaticClass()));

		if(StateComponent&& StateComponent->CheckCurrentState(EState::Death))
		{
			return;
		}

		_OtherActor->AddComponentByClass(Buff_, false, FTransform(), false);
		Destroy();
	}
}

