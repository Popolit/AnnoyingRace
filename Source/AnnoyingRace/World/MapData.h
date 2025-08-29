#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapData.generated.h"

/**
 * Map Data Asset
 */
UCLASS(BlueprintType)
class ANNOYINGRACE_API UMapData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText MapDisplayName_;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> MapThumbnail_;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId MapAssetId_;
};