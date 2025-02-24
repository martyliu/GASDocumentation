// Copyright 2023 Dan Kestranek.


#include "GDGameplayTag.h"
#include "GameplayTagsManager.h"

GDGameplayTags GDGameplayTags::_instance;

void GDGameplayTags::InitializeNativeGameplayTag()
{

	_instance.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("input rmb"));
	_instance.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("input rmb"));
	_instance.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("input key 1"));
	_instance.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("input key 2"));
	_instance.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("input key 3"));
	_instance.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("input key 4"));
	_instance.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.5"), FString("input key 5"));
	_instance.InputTag_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.6"), FString("input key 6"));
	
	// _instance.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Q"), FString("input q"));
	// _instance.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.E"), FString("input e"));
	// _instance.InputTag_F = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.F"), FString("input f"));
	
}

GDGameplayTags::GDGameplayTags()
{


	
}

GDGameplayTags::~GDGameplayTags()
{
}
