// Luis Correa

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "AbilitySystemInterface.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if(InstantEffectApplicationPolicy == E_EffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantEffectToApply);

		if(DestroyOnRemoved)
			Destroy();
	}

	if(DurationEffectApplicationPolicy == E_EffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationEffectToApply);

		if(DestroyOnRemoved)
			Destroy();
	}

	if(InfiniteEffectApplicationPolicy == E_EffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteEffectToApply);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(InstantEffectApplicationPolicy == E_EffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantEffectToApply);

		if(DestroyOnRemoved)
			Destroy();
	}

	if(DurationEffectApplicationPolicy == E_EffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationEffectToApply);

		if(DestroyOnRemoved)
			Destroy();
	}

	if(InfiniteEffectApplicationPolicy == E_EffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteEffectToApply);
	}

	if(InfiniteEffectRemovalPolicy == E_EffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent * ASCRef = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		if(!IsValid(ASCRef))
			return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		
		for(auto HandlePair : ActiveEffectHandles)
		{
			if(ASCRef == HandlePair.Value)
			{
				ASCRef->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for(auto HandleToRemove : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(HandleToRemove);
		}

		if(DestroyOnRemoved)
			Destroy();
	}
}


// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{

	UAbilitySystemComponent * ASCRef = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	if(ASCRef)
	{
		
		FGameplayEffectContextHandle EffectContextHandle = ASCRef->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle EffectSpecHandle;
		
		if(GameplayEffectClass)
		{
			EffectSpecHandle = ASCRef->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
			FActiveGameplayEffectHandle ActiveEffectHandle = ASCRef->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

			const bool IsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

			if(IsInfinite && InfiniteEffectRemovalPolicy == E_EffectRemovalPolicy::RemoveOnEndOverlap)
			{
				ActiveEffectHandles.Add(ActiveEffectHandle, ASCRef);
			}
			
		}
	}
	
}




