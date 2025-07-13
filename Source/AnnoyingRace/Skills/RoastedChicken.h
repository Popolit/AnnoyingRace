#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoastedChicken.generated.h"

UCLASS(Blueprintable, HideDropdown)
class ANNOYINGRACE_API ARoastedChicken : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoastedChicken();

protected:
	virtual void BeginPlay() override;


	UFUNCTION()
		void OnCollisionOverlapped(AActor* _OverlappedActor, AActor* _OtherActor);

private:
	UPROPERTY(EditDefaultsOnly)
		float Lifespan_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UBuffComponent> Buff_;

	UPROPERTY(VisibleDefaultsOnly)
		TObjectPtr<UStaticMeshComponent> MeshComponent_;
};
