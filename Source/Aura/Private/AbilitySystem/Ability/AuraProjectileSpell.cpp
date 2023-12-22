// Luis Correa


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/AuraCombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKismetSystemLibrary::PrintString(this, "Ability Activated");

	const bool bIsServer = HasAuthority(&ActivationInfo);

	if(!bIsServer)
		return;
	
	AActor* OwningActor = GetOwningActorFromActorInfo();
	APawn* OwningPawn = Cast<APawn>(OwningActor);
	
	IAuraCombatInterface * CombatInterface = Cast<IAuraCombatInterface>(GetAvatarActorFromActorInfo());

	if(CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		
		AAuraProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, OwningActor, OwningPawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
	

	
		
}
