// Luis Correa


#include "Character/AuraEnemy.h"

#include "AuraGameplaytags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Weapon->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_HIGHLIGHT);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_HIGHLIGHT);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
	InitializeDefaultAttributes();
	ApplyDefaultEffects();

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting? 0 : BaseWalkSpeed;
}

void AAuraEnemy::BroadcastInitialValues()
{
	if(const UAuraAttributeSet * AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	}
}

void AAuraEnemy::BindCallbacksToDependencies()
{
	const UAuraAttributeSet * AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	//register to the hit react tag added event
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::HitReactTagChanged);
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 Count)
{
	bHitReacting = Count > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting? 0 : BaseWalkSpeed;
	
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifespanAfterDeath);

	UMaterialInstanceDynamic * DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0, CharacterDissolveMaterial);
	GetMesh()->SetMaterial(0, DynamicMaterial);
	StartDissolveEffect(DynamicMaterial);;
	Super::Die();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
	ApplyDefaultEffects();
	BroadcastInitialValues();
	BindCallbacksToDependencies();
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	//Super::InitializeDefaultAttributes();

	UCharacterClassInfo * CharacterClassDefaultParams = UAuraAbilitySystemLibrary::GetCharacterClassInfo(this);
	
	if(CharacterClassDefaultParams)
	{
		const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassDefaultParams->GetClassDefaultInfo(CharacterClass);

		
		ApplyGameplayEffectToSelf(CharacterClassDefaultInfo.PrimaryAttributes);
		ApplyGameplayEffectToSelf(CharacterClassDefaultParams->SecondaryAttributes);
		ApplyGameplayEffectToSelf(CharacterClassDefaultParams->VitalAttributes);

		UAuraAbilitySystemComponent * ASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);

		ASC->GrantAbilitiesFromArray(CharacterClassDefaultParams->CommonAbilities);
	}
}

