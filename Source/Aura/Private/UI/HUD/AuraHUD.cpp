// Luis Correa


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetWidgetController(const FWidgetControllerParams& Params)
{
	if(OverlayWidgetControllerPtr ==  nullptr)
	{
		OverlayWidgetControllerPtr = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetControllerPtr->SetWidgetControllerParams(Params);
		OverlayWidgetControllerPtr->BindCallbacksToDependencies();
	}

	return OverlayWidgetControllerPtr;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributesWidgetController(const FWidgetControllerParams& Params)
{
	if(AttributesWidgetControllerPtr == nullptr)
	{
		AttributesWidgetControllerPtr = NewObject<UAttributeMenuWidgetController>(this, AttributesWidgetControllerClass);
		AttributesWidgetControllerPtr->SetWidgetControllerParams(Params);
		AttributesWidgetControllerPtr->BindCallbacksToDependencies();
	}

	return AttributesWidgetControllerPtr;
}

void AAuraHUD::InitOverlay(APlayerController* PController, APlayerState* PState,
                           UAbilitySystemComponent* AbilitySystemComp, UAttributeSet* AttribSet)
{

	checkf(OverlayWidgetClass, TEXT("Overlay widget class not set"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class not set"));

	UUserWidget * Widget = CreateWidget(GetWorld(), OverlayWidgetClass);

	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetParams(PController, PState, AbilitySystemComp, AttribSet);
	
	UOverlayWidgetController * WidgetControllerPtr = GetWidgetController(WidgetParams);
	OverlayWidget->SetWidgetController(WidgetControllerPtr);

	WidgetControllerPtr->BroadcastInitialValues();
	
	Widget->AddToViewport();
	
}
