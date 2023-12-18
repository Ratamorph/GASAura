// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAbilityInputConfig.generated.h"

struct FGameplayTag;

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
	
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilityInputConfig : public UDataAsset
{
	GENERATED_BODY()

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLOhNotFound = false) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
	
};
