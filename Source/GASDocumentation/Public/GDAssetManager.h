// Copyright 2023 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GDAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GASDOCUMENTATION_API UGDAssetManager : public UAssetManager
{
	GENERATED_BODY()

	static UGDAssetManager& Get();
	virtual void StartInitialLoading() override;
};
