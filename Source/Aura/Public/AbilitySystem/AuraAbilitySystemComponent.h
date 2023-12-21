// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

	void GrantAbilitiesFromArray(const TArray<TSubclassOf<UGameplayAbility>>& AbilityArray);

	void AbilityInputTagHeld(FGameplayTag& InputTag);
	void AbilityInputTagReleased(FGameplayTag& InputTag);
	
protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent * AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle);
	
};
