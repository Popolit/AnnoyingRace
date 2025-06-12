#include "HitBox.h"

#include "Components/ShapeComponent.h"
#include "Components/SkillComponent.h"
#include "Kismet/GameplayStatics.h"


void AHitBox::BeginPlay()
{
	Super::BeginPlay();

	for(auto Elem : GetComponentsByClass(UShapeComponent::StaticClass()))
	{
		UShapeComponent* Collision = Cast<UShapeComponent>(Elem);
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if(Collision)
		{
			Collision->OnComponentBeginOverlap.AddDynamic(this, &AHitBox::OnCollisionOverlapped);

#if WITH_EDITOR
			Collision->bHiddenInGame = false;
#endif
		}
	}
}

void AHitBox::EnableCollisions()
{
	for (auto Elem : GetComponentsByClass(UShapeComponent::StaticClass()))
	{
		UShapeComponent* Collision = Cast<UShapeComponent>(Elem);
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (Collision)
		{
			Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

void AHitBox::OnCollisionOverlapped(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	AActor* OwnerActor = GetOwner();
	if(_OtherActor == OwnerActor)
	{
		return;
	}

	check(OwnerActor);

	const USkillComponent* SkillComp = Cast<USkillComponent>(OwnerActor->GetComponentByClass(USkillComponent::StaticClass()));
	check(SkillComp);

	FPointDamageEvent DamageEvent;
	DamageEvent.HitInfo = _SweepResult;
	UGameplayStatics::ApplyPointDamage(_OtherActor, SkillComp->GetSkillDamage(), _SweepResult.TraceStart - _SweepResult.TraceEnd, _SweepResult, OwnerActor->GetInstigatorController(), OwnerActor, DamageEvent.DamageTypeClass);
}
