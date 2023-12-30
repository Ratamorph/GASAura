// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UAuraCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IAuraCombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();
	virtual FVector GetCombatSocketLocation();

	UFUNCTION(Blueprintable, BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(FVector FacingTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage * GetHitReact();

	virtual void Die() = 0;
};
