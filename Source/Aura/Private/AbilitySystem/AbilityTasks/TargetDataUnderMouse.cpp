// Luis Correa


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::GetTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	/*
	const APlayerController* Controller = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	Controller->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	
	Data->HitResult = CursorHit;
	TargetData.Broadcast(Data);
	*/
	

	const bool bLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	
	if(bLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		//do server stuff, listen for target data

		
		const FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(AbilitySpecHandle, PredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle, PredictionKey);

		if(!bCalledDelegate)
			SetWaitingOnRemotePlayerData();
		
	}
	
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	const APlayerController* Controller = Ability->GetCurrentActorInfo()->PlayerController.Get();
	
	FHitResult HitResult;
	Controller->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	
	Data->HitResult = HitResult;
	TargetDataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), TargetDataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);
	
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		TargetData.Broadcast(TargetDataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if(ShouldBroadcastAbilityTaskDelegates())
		TargetData.Broadcast(DataHandle);
}
