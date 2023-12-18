// Luis Correa


#include "AbilitySystem/Input/AuraAbilityInputConfig.h"

#include "InputAction.h"

const UInputAction* UAuraAbilityInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                    bool bLOhNotFound) const
{
	for (auto InputActionConfig : AbilityInputActions)
	{
		if(InputActionConfig.InputAction && InputActionConfig.InputTag == InputTag)
		{
			return InputActionConfig.InputAction;
		}
	}

	if(bLOhNotFound)
		UE_LOG(LogTemp, Error, TEXT("Cound not found input for tag [%s] on AbilityInputActions on [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}
