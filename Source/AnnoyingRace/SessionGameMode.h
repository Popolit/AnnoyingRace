#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SessionGameMode.generated.h"

class FNamedOnlineSession;
/**
 * Session Game Mode
 */
UCLASS()
class ANNOYINGRACE_API ASessionGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ASessionGameMode();
	
protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* _NewPlayer) override;

	virtual void Logout(AController* _ExitPlayer) override;

public:
	void SetSessionJoinable(bool _bCanJoin);

	void MakeSessionPublic();
	
	void RequestChat(const FText& _Chat, const FString& _ChatterName);

private:
	void OnPostLoadMap(UWorld* _World);

	void InitializeSession();

	void OnSessionUpdated(FName _SessionName, bool _bWasSuccessful);

	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> GetSessionInterface();
};
