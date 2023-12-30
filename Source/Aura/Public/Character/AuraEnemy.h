// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
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

	UFUNCTION(BlueprintImplementableEvent, Category ="Effects|Death")
	void StartDissolveEffect(UMaterialInstanceDynamic* DynamicDissolveMaterial);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character attributes")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character attributes")
	TEnumAsByte<ECharacterClass> CharacterClass;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifespanAfterDeath = 5.f;
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 Count);

	virtual void Die() override;
	
private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditDefaultsOnly, Category="Character attributes")
	TObjectPtr<UCharacterClassInfo> CharacterClassDefaultParams;

	
protected:
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UMaterialInstance> CharacterDissolveMaterial;
	
};
