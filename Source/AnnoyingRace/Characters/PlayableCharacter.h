#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnCharacterDied, ACharacter*);
DECLARE_DELEGATE_OneParam(FOnSkillButtonPushed, ACharacter*);

UCLASS()
class ANNOYINGRACE_API APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayableCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent) override;

	virtual float TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser) override;

	void ProcessHit(uint8 _DamageAmount, FDamageEvent const& _DamageEvent);

	void ProcessDeath();

	void SkillButtonPushed();

private:
	void CreateAllComponents();


public:
	FOnCharacterDied OnCharacterDied_;
	FOnSkillButtonPushed OnSkillButtonPushed_;

	//Components
private:
	UPROPERTY(VisibleDefaultsOnly)
		TObjectPtr<class USpringArmComponent> SpringArmComponent_;
	UPROPERTY(VisibleDefaultsOnly)
		TObjectPtr<class UCameraComponent> CameraComponent_;
	UPROPERTY(VisibleDefaultsOnly)
		TObjectPtr<class UStateComponent> StateComponent_;
	UPROPERTY(VisibleDefaultsOnly)
		TObjectPtr<class USkillComponent> SkillComponent_;

	//Inputs
private:
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputMappingContext> IMC_;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_Move_;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_Look_;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_UseSkill_;

private:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		TObjectPtr<UAnimMontage> HitAnimation_;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		TObjectPtr<UAnimMontage> DeathAnimation_;

	UPROPERTY(EditDefaultsOnly)
		uint8 HP_;
};