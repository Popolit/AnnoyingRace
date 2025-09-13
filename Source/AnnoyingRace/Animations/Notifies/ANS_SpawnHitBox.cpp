#include "ANS_SpawnHitBox.h"
#include "Skills/HitBoxes/HitBox.h"


void UANS_SpawnHitBox::NotifyBegin(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EventReference)
{
    check(_MeshComp);

	//에디터에서 미리보기 히트박스를 볼 수 있음
    if (false == _MeshComp->HasBegunPlay())
    {
    	if (HitBoxClass_)
    	{
    		HitBox_ = Cast<AHitBox>(_MeshComp->GetWorld()->SpawnActor(HitBoxClass_));
    	}
        return;
    }

	AActor* OwnerActor = _MeshComp->GetOwner();
    if(OwnerActor && OwnerActor->HasAuthority())
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

void UANS_SpawnHitBox::NotifyEnd(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation,  const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = _MeshComp->GetOwner();
	if (OwnerActor && OwnerActor->HasAuthority())
	{
		if (HitBox_)
		{
			HitBox_->Destroy();
		}
		HitBox_ = nullptr;
	}
}
