#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	Idle    UMETA(DisplayName = "Idle"),
	Hit    UMETA(DisplayName = "Hit"),
	Skill  UMETA(DisplayName = "Skill"),
	Death	UMETA(DisplayName = "Death"),
};


/*
 * States for Character
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANNOYINGRACE_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateComponent();

	virtual void BeginPlay() override;

	bool CheckCurrentState(EState _State);

	void SetState(EState _NewState);


public:
	//Enhanced Input Actions;
	void Move(const struct FInputActionInstance& _Instance);
	void Look(const struct FInputActionInstance& _Instance);
	void SkillButtonPushed(const struct FInputActionInstance& _Instance);

	void TakeDamage(ACharacter* _Character, float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser);

private:
	TObjectPtr<class UState> CurrentState_;

	UPROPERTY()
		TMap<EState, TObjectPtr<UState>> States_;
};
