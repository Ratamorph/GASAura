// Luis Correa


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplaytags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Input/AuraEnhancedInputComponent.h"
#include "Components/SplineComponent.h"
#include "Interaction/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem * SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(SubSystem)
	{
		SubSystem->AddMappingContext(AuraContext,0);
		
		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Default;

		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeData.SetHideCursorDuringCapture(false);
		SetInputMode(InputModeData);
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraEnhancedInputComponent * EnhancedInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	EnhancedInputComponent->BindAbilityActions(AbilityInputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	CursorTrace();
	AutoRunUpdate();
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation (0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn * ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	if(!CursorHit.bBlockingHit)
		return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
			ThisActor->HighlightActor();
	}
	else
	{
		if(ThisActor == nullptr)
			LastActor->UnHighlightActor();
		else
		{
			if(LastActor != ThisActor)
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
	
}

void AAuraPlayerController::AutoRunUpdate()
{
	if(!bAutoRunning)
		return;
	
	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();

		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}

	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_Primary))
	{
		bTargeting = ThisActor? true : false;
		bAutoRunning = false;
	}
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_Primary))
	{
		if(GetASC())
			GetASC()->AbilityInputTagReleased(InputTag);

		return;
	}

	if(bTargeting)
	{
		if(GetASC())
			GetASC()->AbilityInputTagReleased(InputTag);
	}
	else
	{
		APawn * ControlledPawn = GetPawn();
		
		if(FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if(UNavigationPath * NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();

				for (FVector Point : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
					//DrawDebugSphere(GetWorld(), Point, 8, 8, FColor::Green, false, 5);
				}

				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1]; //cover case where we click on an unreachable location
				bAutoRunning = true;
			}
			
		}

		bTargeting = false;
		FollowTime = 0.f;
		
	}
	
	//GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_Primary))
	{
		if(GetASC())
			GetASC()->AbilityInputTagHeld(InputTag);

		return;
	}

	if(bTargeting)
	{
		if(GetASC())
			GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if(CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if(APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
		
		
	}
	
	//GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
}
