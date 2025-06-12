#include "ANS_SpawnHitBox.h"

#include "Components/SkillComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Skills/HitBoxes/HitBox.h"


void UANS_SpawnHitBox::NotifyBegin(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation,
                                   float _TotalDuration)
{
    check(_MeshComp);

    if(_MeshComp->HasBegunPlay())
    {
        if(ensureMsgf(HitBoxClass_, TEXT("%s's HitBox class was not set"), *this->GetName()))
        {
            HitBox_ = Cast<AHitBox>(_MeshComp->GetWorld()->SpawnActor(HitBoxClass_));
            check(HitBox_);

            HitBox_->SetOwner(_MeshComp->GetOwner());
            HitBox_->AttachToComponent(_MeshComp, FAttachmentTransformRules::KeepRelativeTransform);

            HitBox_->EnableCollisions();
        }
    }
}

void UANS_SpawnHitBox::NotifyEnd(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation)
{
    if(HitBox_)
    {
		HitBox_->Destroy();
    }
    HitBox_ = nullptr;
}
