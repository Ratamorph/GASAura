// Luis Correa


#include "AuraGameplaytags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GamePlayTagsInstance; //this is needed


void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	
	GamePlayTagsInstance.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Strength"));
	GamePlayTagsInstance.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Intelligence"));
	GamePlayTagsInstance.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), FString("Resilience"));
	GamePlayTagsInstance.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("Vigor"));

	GamePlayTagsInstance.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduce damage taken, improves block chance"));
	GamePlayTagsInstance.Attributes_Primary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), FString("ArmorPenetration"));
	GamePlayTagsInstance.Attributes_Primary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("BlockChance"));
	GamePlayTagsInstance.Attributes_Primary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("CriticalHitChance"));
	GamePlayTagsInstance.Attributes_Primary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("CriticalHitDamage"));
	GamePlayTagsInstance.Attributes_Primary_CriticalHitResistence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistence"), FString("CriticalHitResistence"));
	GamePlayTagsInstance.Attributes_Primary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("HealthRegeneration"));
	GamePlayTagsInstance.Attributes_Primary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("ManaRegeneration"));
	GamePlayTagsInstance.Attributes_Primary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("MaxHealth"));
	GamePlayTagsInstance.Attributes_Primary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("MaxMana"));

	GamePlayTagsInstance.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Fire resistance"));
	GamePlayTagsInstance.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString("Arcane resistance"));
	GamePlayTagsInstance.Attributes_Resistance_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lighting"), FString("Lighting resistance"));
	GamePlayTagsInstance.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Physical resistance"));

	
	GamePlayTagsInstance.Input_Primary = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Primary"), FString("Primary input"));
	GamePlayTagsInstance.Input_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Secondary"), FString("Secondary input"));
	GamePlayTagsInstance.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.1"), FString("Ability 1"));
	GamePlayTagsInstance.Input_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.2"), FString("Ability 2"));
	GamePlayTagsInstance.Input_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.3"), FString("Ability 3"));
	GamePlayTagsInstance.Input_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.4"), FString("Ability 4"));

	GamePlayTagsInstance.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
	GamePlayTagsInstance.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage"));
	GamePlayTagsInstance.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), FString("Arcane Damage"));
	GamePlayTagsInstance.Damage_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lighting"), FString("Lighting Damage"));
	GamePlayTagsInstance.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical Damage"));

	GamePlayTagsInstance.DamageTypeToResistance.Add(GamePlayTagsInstance.Damage_Fire, GamePlayTagsInstance.Attributes_Resistance_Fire);
	GamePlayTagsInstance.DamageTypeToResistance.Add(GamePlayTagsInstance.Damage_Arcane, GamePlayTagsInstance.Attributes_Resistance_Arcane);
	GamePlayTagsInstance.DamageTypeToResistance.Add(GamePlayTagsInstance.Damage_Lighting, GamePlayTagsInstance.Attributes_Resistance_Lighting);
	GamePlayTagsInstance.DamageTypeToResistance.Add(GamePlayTagsInstance.Damage_Physical, GamePlayTagsInstance.Attributes_Resistance_Physical);

	
	GamePlayTagsInstance.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"), FString("Hit React tag"));
	GamePlayTagsInstance.Effect_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.Death"), FString("Death tag"));

}
