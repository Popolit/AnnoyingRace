// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SetState_Skill.generated.h"

/**
 * Set State to Skill
 */
UCLASS()
class ANNOYINGRACE_API UAN_SetState_Skill : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation) override;
};
