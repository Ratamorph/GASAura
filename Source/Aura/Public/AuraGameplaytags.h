// Luis Correa

#pragma once

#include "GameplayTagContainer.h"
#include "CoreMinimal.h"

/*
 * Aura gameplay tags singleton
 */

struct FAuraGameplayTags
{
	
public:
	static const FAuraGameplayTags& Get(){return GamePlayTagsInstance; }
	static void InitializeNativeGameplayTags();

	//primary
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	//secondary
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Primary_ArmorPenetration;
	FGameplayTag Attributes_Primary_BlockChance;
	FGameplayTag Attributes_Primary_CriticalHitChance;
	FGameplayTag Attributes_Primary_CriticalHitDamage;
	FGameplayTag Attributes_Primary_CriticalHitResistence;
	FGameplayTag Attributes_Primary_HealthRegeneration;
	FGameplayTag Attributes_Primary_ManaRegeneration;
	FGameplayTag Attributes_Primary_MaxHealth;
	FGameplayTag Attributes_Primary_MaxMana;


protected:
	
private:
	static FAuraGameplayTags GamePlayTagsInstance;
};
