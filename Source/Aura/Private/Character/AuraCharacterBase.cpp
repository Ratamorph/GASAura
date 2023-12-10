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

void AAuraCharacterBase::InitializePrimaryAttributes()
{
	if(!DefaultPrimaryAttributes)
		return;

	check(GetAbilitySystemComponent());
	
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle;

	EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

