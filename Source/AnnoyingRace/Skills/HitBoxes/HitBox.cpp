#include "HitBox.h"

#include "Components/ShapeComponent.h"
#include "Components/SkillComponent.h"
#include "Skills/RaceDamageEvent.h"


AHitBox::AHitBox()
{
	bReplicates = true;
}

void AHitBox::BeginPlay()
{
	Super::BeginPlay();
	TArray<UShapeComponent*> ShapeComponents;
	GetComponents<UShapeComponent>(ShapeComponents);

	for(auto Elem : ShapeComponents)
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
	TArray<UShapeComponent*> ShapeComponents;
	GetComponents<UShapeComponent>(ShapeComponents);

	for (auto Elem : ShapeComponents)
	{
		UShapeComponent* Collision = Cast<UShapeComponent>(Elem);
		if (Collision)
		{
			Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

void AHitBox::OnCollisionOverlapped(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	if (false == HasAuthority())
	{
		return;
	}
	
	AActor* OwnerActor = GetOwner();
	check(OwnerActor);
	if(_OtherActor == OwnerActor)
	{
		return;
	}
	
	const USkillComponent* SkillComp = Cast<USkillComponent>(OwnerActor->GetComponentByClass(USkillComponent::StaticClass()));
	check(SkillComp);

	FRaceDamageEvent DamageEvent;
	DamageEvent.KnockbackVector_ = KnockbackVector_;
	_OtherActor->TakeDamage(SkillComp->GetSkillDamage(), DamageEvent, OwnerActor->GetInstigatorController(), OwnerActor);
}
