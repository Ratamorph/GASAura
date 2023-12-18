// Luis Correa


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WoldContext)
{
	if(AAuraPlayerController * AuraPlayerController = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(WoldContext, 0)))
	{
		if(AAuraHUD * AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AAuraPlayerState * PS = AuraPlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent * ASC = PS->GetAbilitySystemComponent();
			
			return AuraHUD->GetWidgetController(FWidgetControllerParams(AuraPlayerController, PS, ASC, PS->GetAttributeSet()));
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WoldContext)
{
	if(AAuraPlayerController * AuraPlayerController = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(WoldContext, 0)))
	{
		if(AAuraHUD * AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AAuraPlayerState * PS = AuraPlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent * ASC = PS->GetAbilitySystemComponent();
			
			return AuraHUD->GetAttributesWidgetController(FWidgetControllerParams(AuraPlayerController, PS, ASC, PS->GetAttributeSet()));
		}
	}

	return nullptr;
}
