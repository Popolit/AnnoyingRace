#include "MapData.h"

FPrimaryAssetId UMapData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("MapData", GetFName());
}
