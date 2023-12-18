// Luis Correa


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//init Actor info for the server
	InitAbilityActorInfo();
	GrantInitialAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	//init Actor infor for client
	InitAbilityActorInfo();
}


int32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState * AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	check(AuraPlayerState);
	
	return AuraPlayerState->GetPlayerLevel();
}


void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState * AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	AAuraPlayerController * PlayerController = Cast<AAuraPlayerController>(GetController());

	if(PlayerController)
	{
		AAuraHUD * Hud = Cast<AAuraHUD>(PlayerController->GetHUD());

		if(Hud)
			Hud->InitOverlay(PlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
	}

	InitializeDefaultAttributes();
	ApplyDefaultEffects();
}
