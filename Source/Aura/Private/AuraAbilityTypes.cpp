#include "AuraAbilityTypes.h"

UScriptStruct* FAuraGameplayEffectContext::GetScriptStruct() const
{
	//return FGameplayEffectContext::GetScriptStruct();
	return FGameplayEffectContext::StaticStruct();
}

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return true;
}
