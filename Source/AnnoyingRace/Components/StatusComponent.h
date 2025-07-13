#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANNOYINGRACE_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float GetSpeed() const;

	void AddSpeedBonus(float _SpeedBonus);

	void MultiplySpeedBonus(float _SpeedBonus);

	void DamageToHP(float _Amount);

private:
	UFUNCTION()
		void OnRep_Speed();

private:
	UPROPERTY(EditDefaultsOnly, Replicated)
		float HP_;

	UPROPERTY(EditDefaultsOnly)
		float BaseSpeed_;

	UPROPERTY(ReplicatedUsing = OnRep_Speed)
		float AddedSpeedBonus_;

	UPROPERTY(ReplicatedUsing = OnRep_Speed)
		float MultipliedSpeedBonus_;
};
