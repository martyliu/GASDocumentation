// Copyright 2023 Dan Kestranek.


#include "Input/GDInputConfig.h"

const UInputAction* UGDInputConfig::GetInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{

	for (const FGDInputAction &Action : InputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find input action for tag [%s] on input config [%s]"), *InputTag.ToString(), *GetName());
	}

	return nullptr;
}
