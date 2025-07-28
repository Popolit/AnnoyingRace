#include "CheckPointBox.h"

#include "RaceGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"

ACheckPointBox::ACheckPointBox()
{
	SetRootComponent(Root_);
	Camera_ = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera_->SetupAttachment(Root_);
	Camera_->SetRelativeLocation(FVector::ZeroVector);
}

void ACheckPointBox::BeginPlay()
{
	Super::BeginPlay();

	UShapeComponent* Collision = GetCollisionComponent();

	if(ensureMsgf(Collision, TEXT("%s's Collision was nullptr"), *this->GetName()))
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ACheckPointBox::OnCollisionOverlapped);
	}
}

void ACheckPointBox::OnCollisionOverlapped(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor,
                                    UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	ACharacter* OverlappedCharacter = Cast<ACharacter>(_OtherActor);

	//시체 혹은 다른 오브젝트일 경우 return
	if(nullptr == OverlappedCharacter || false == OverlappedCharacter->IsPlayerControlled())
	{
		return;
	}

	//서버 처리
	if(HasAuthority())
	{
		ARaceGameMode* GM = Cast<ARaceGameMode>(GetWorld()->GetAuthGameMode());
		if(GM)
		{
			APlayerController* PC = Cast<APlayerController>(OverlappedCharacter->GetController());
			check(PC);

			GM->HandleCheckPointPassed(Index_, PC);
		}
	}

}
