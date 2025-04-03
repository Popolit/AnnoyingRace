#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacter.generated.h"

UCLASS()
class ANNOYINGRACE_API APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayableCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent) override;

private:
	void CreateAllComponents();

private:
	//Enhanced Input Actions;
	void Move(const struct FInputActionInstance& _Instance);
	void Look(const struct FInputActionInstance& _Instance);
	void UseSkill(const struct FInputActionInstance& _Instance);

	//Components
private:
	UPROPERTY(VisibleDefaultsOnly)
		TObjectPtr<class USpringArmComponent> SpringArmComponent_;
	UPROPERTY(VisibleDefaultsOnly)
		TObjectPtr<class UCameraComponent> CameraComponent_;

	//Inputs
private:
	UPROPERTY(EditDefaultsOnly, Category=Input)
		TObjectPtr<class UInputMappingContext> IMC_;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_Move_;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_Look_;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_UseSkill_;

private:
	UPROPERTY(EditDefaultsOnly, Category = Skill)
		TObjectPtr<class USkill> Skill_;
};