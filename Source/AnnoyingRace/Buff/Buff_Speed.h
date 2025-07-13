#pragma once

#include "CoreMinimal.h"
#include "Components/BuffComponent.h"
#include "Buff_Speed.generated.h"

/**
 * Buff for speed
 */
UCLASS(Blueprintable, HideDropdown)
class ANNOYINGRACE_API UBuff_Speed : public UBuffComponent
{
	GENERATED_BODY()

public:
	UBuff_Speed();

private:
	virtual void Buff(AActor* _OwnerActor) override;

	virtual void OnBuffEnd(AActor* _OwnerActor) override;

private:
	UPROPERTY(EditDefaultsOnly)
		float Amount_;

	//True = Multiply, False = Add
	UPROPERTY(EditDefaultsOnly)
		bool bMultiply;
};
