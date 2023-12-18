// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAuraWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAuraUserWidget;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UOverlayWidgetController * GetWidgetController(const FWidgetControllerParams& Params);
	UAttributeMenuWidgetController * GetAttributesWidgetController(const FWidgetControllerParams& Params);

	
	void InitOverlay(APlayerController * PController, APlayerState * PState, UAbilitySystemComponent * AbilitySystemComp, UAttributeSet * AttribSet);

private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetControllerPtr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributesWidgetControllerPtr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController> AttributesWidgetControllerClass;
};
