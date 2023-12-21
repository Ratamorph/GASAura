// Luis Correa


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::GrantAbilitiesFromArray(const TArray<TSubclassOf<UGameplayAbility>>& AbilityArray)
{
	for (const auto Ability : AbilityArray)
	{
		FGameplayAbilitySpec AbilitySpec = BuildAbilitySpecFromClass(Ability, 1);
		
		if(const UAuraGameplayAbility *AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
		}

		GiveAbility(AbilitySpec);
		//GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())
		return;
	
	for (auto AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			
			if(!AbilitySpec.IsActive())
				TryActivateAbility(AbilitySpec.Handle);
		}
		
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())
		return;

	for (auto AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			
		}
		
	}
}

//needs the _Implementation because it's an RPC
void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
}
