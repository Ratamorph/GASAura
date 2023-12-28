// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyOnAttributeChangedSignature, float, NewValue);

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual int32 GetPlayerLevel() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FEnemyOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FEnemyOnAttributeChangedSignature OnMaxHealthChanged;
	
	virtual void BeginPlay() override;
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character attributes")
	int32 Level = 1;

private:
	virtual void InitAbilityActorInfo() override;
};
