// Copyright 2023 Dan Kestranek.


#include "GDAssetManager.h"

#include "GDGameplayTag.h"

UGDAssetManager& UGDAssetManager::Get()
{
	check(GEngine);
	
	UGDAssetManager* gdAssetManager = Cast<UGDAssetManager>(GEngine->AssetManager);
	return *gdAssetManager;
}

void UGDAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	GDGameplayTags::Get().InitializeNativeGameplayTag();
}
