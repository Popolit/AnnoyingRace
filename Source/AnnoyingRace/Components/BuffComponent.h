#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANNOYINGRACE_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuffComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	TSoftObjectPtr<UTexture2D> GetBuffImg();

	virtual void BeginPlay() override;

	virtual void TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction) override;

private:
	UFUNCTION()
		void OnRep_BuffRemain();

	virtual void Buff(AActor* _OwnerActor) {}

	virtual void OnBuffEnd(AActor* _OwnerActor) {}

private:
	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<UTexture2D> BuffImg_;

	//Set -1 = Inf
	UPROPERTY(EditDefaultsOnly)
		float BuffDuration_;

	UPROPERTY(ReplicatedUsing = OnRep_BuffRemain)
		float BuffRemain_;
};
