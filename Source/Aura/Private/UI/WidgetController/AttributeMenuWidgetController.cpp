// Luis Correa


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplaytags.h"
#include "AbilitySystem/AttributeInfo.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	
	UAuraAttributeSet * AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	for(auto& Pair : AS->TagsToAttributesMap)
	{
		BroadcastAttributeDefinition(Pair.Key, Pair.Value());
	}
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
	UAuraAttributeSet * AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	for(auto& Pair : AS->TagsToAttributesMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeDefinition(Pair.Key, Pair.Value());
			}
		);
		
	}
	
}

void UAttributeMenuWidgetController::BroadcastAttributeDefinition(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoFromTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
