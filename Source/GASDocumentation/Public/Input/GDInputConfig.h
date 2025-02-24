// Copyright 2023 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "GDInputConfig.generated.h"



USTRUCT(BlueprintType)
struct FGDInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	const UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	FGameplayTag InputTag;
};

/**
 * 
 */
UCLASS()
class GASDOCUMENTATION_API UGDInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* GetInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<FGDInputAction> InputActions;
	
};
