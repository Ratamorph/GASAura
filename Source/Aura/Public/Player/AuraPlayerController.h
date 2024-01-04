// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UCharacterClassInfo;
class UDamageTextWidgetComponent;
class USplineComponent;
struct FGameplayTag;
class UAuraAbilityInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();

	void SpawnDamageText(float Damage, bool BlockedHit, bool CriticalHit, ACharacter * TargetCharacter);

	UCharacterClassInfo* GetCharacterClassInfo();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();
	void AutoRunUpdate();

	void OnShiftPressed() { bShiftHeld = true; }
	void OnShiftReleased() { bShiftHeld = false; }
	bool bShiftHeld = false;

	IEnemyInterface * LastActor;
	IEnemyInterface * ThisActor;
	FHitResult CursorHit;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent *GetASC();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraAbilityInputConfig> AbilityInputConfig;
	
	FVector CachedDestination = FVector::Zero();
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UDamageTextWidgetComponent> FloatingTextComponentClass;

	UPROPERTY(EditDefaultsOnly, Category="Character attributes")
	TObjectPtr<UCharacterClassInfo> CharacterClassDefaultParams;

};
