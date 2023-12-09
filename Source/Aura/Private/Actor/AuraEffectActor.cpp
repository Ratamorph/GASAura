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
			EffectSpecHandle = ASCRef->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
			ASCRef->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}




