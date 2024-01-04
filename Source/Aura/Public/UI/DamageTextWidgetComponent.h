// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintable, BlueprintImplementableEvent)
	void SetDamageText(float Damage, bool BlockedHit, bool CriticalHit);
};
