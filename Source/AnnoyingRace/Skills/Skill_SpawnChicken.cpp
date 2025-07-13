#include "Skill_SpawnChicken.h"

#include "RoastedChicken.h"
#include "GameFramework/Character.h"
#include "Triggers/Trigger_Death.h"

void USkill_SpawnChicken::Initialize(ACharacter* _Character)
{
	SkillTrigger_ = NewObject<UTrigger_Death>();

	Super::Initialize(_Character);
}

void USkill_SpawnChicken::TriggerSkill(ACharacter* _Character)
{
	TSubclassOf<ARoastedChicken> LoadedClass = RoastedChickenClass_.LoadSynchronous();
	FTransform SpawnTransform = _Character->GetActorTransform();
	FVector SpawnLocation = SpawnTransform.GetLocation();
	SpawnLocation.Z = 0;
	SpawnTransform.SetLocation(SpawnLocation);
	GetWorld()->SpawnActor<ARoastedChicken>(LoadedClass, SpawnTransform);
}
