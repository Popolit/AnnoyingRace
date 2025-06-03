#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANNOYINGRACE_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SetTrigger(TFunction<void(ACharacter*)> _Func);

public:
	void SkillButtonPushed(ACharacter* _Character);


private:
	UPROPERTY(EditDefaultsOnly, Category = Skill)
		TSubclassOf<class USkill> SkillClass_;

	UPROPERTY()
		TObjectPtr<USkill> Skill_;

	TFunction<void(ACharacter*)> TriggerFunc_;
};
