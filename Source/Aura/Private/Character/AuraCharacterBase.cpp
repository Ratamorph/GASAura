// Luis Correa


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect) const
{
	if(!Effect)
		return;

	check(GetAbilitySystemComponent());
	
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle;

	EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(Effect, 1, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyGameplayEffectToSelf(DefaultPrimaryAttributes);
	ApplyGameplayEffectToSelf(DefaultSecondaryAttributes);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

int32 AAuraCharacterBase::GetPlayerLevel()
{
	return 1;
}

