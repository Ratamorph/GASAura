// Luis Correa


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "GameplayEffectTypes.h"
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

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	UCharacterClassInfo * CharacterClassInfo = nullptr;

	if(AAuraPlayerController * AuraPlayerController = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(WorldContext, 0)))
	{
		CharacterClassInfo = AuraPlayerController->GetCharacterClassInfo();
	}
		
	return CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		return EffectContext->IsBlockedHit();
	
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		return EffectContext->IsCriticalHit();
	
	return false;
}

void UAuraAbilitySystemLibrary::SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if(FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetIsBlockedHit(bInIsBlockedHit);
}

void UAuraAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if(FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		EffectContext->SetIsCriticalHit(bInIsCriticalHit);
}


