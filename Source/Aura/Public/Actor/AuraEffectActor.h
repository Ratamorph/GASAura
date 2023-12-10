// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;

UENUM(BlueprintType)
enum class E_EffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DontApply
};

UENUM(BlueprintType)
enum class E_EffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DontRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraEffectActor();

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor * TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor * TargetActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor * TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantEffectToApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	E_EffectApplicationPolicy InstantEffectApplicationPolicy = E_EffectApplicationPolicy::DontApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationEffectToApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	E_EffectApplicationPolicy DurationEffectApplicationPolicy = E_EffectApplicationPolicy::DontApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteEffectToApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	E_EffectApplicationPolicy InfiniteEffectApplicationPolicy = E_EffectApplicationPolicy::DontApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	E_EffectRemovalPolicy InfiniteEffectRemovalPolicy = E_EffectRemovalPolicy::RemoveOnEndOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	bool DestroyOnRemoved = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	float ActorLevel = 1.f;
	
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
};
