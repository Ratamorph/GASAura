// Luis Correa


#include "AbilitySystem/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoFromTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for(const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if(Info.AttributeTag == Tag)
			return Info;
	}

	if(bLogNotFound)
		UE_LOG(LogTemp, Error, TEXT("Cound not found attribute for tag [%s] on AttributeInformation on [%s]"), *Tag.ToString(), *GetNameSafe(this));

	return FAuraAttributeInfo();
}
