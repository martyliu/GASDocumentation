// Copyright 2023 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct GASDOCUMENTATION_API GDGameplayTags
{
	
public:
	static const GDGameplayTags Get()
	{
		return _instance;
	}
	static void InitializeNativeGameplayTag();
	

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;

	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_5;
	FGameplayTag InputTag_6;
	FGameplayTag InputTag_7;

	
private:
	static GDGameplayTags _instance;
		
	
	
public:
	GDGameplayTags();
	~GDGameplayTags();
};
