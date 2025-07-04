#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANNOYINGRACE_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void BeginPlay() override;

	uint8 GetSkillDamage() const;

private:
	UFUNCTION()
		void OnRep_Skill();

private:
	UPROPERTY(EditDefaultsOnly, Category = Skill)
		TSubclassOf<class USkill> SkillClass_;

	UPROPERTY(ReplicatedUsing = OnRep_Skill)
		TObjectPtr<USkill> Skill_;
};
