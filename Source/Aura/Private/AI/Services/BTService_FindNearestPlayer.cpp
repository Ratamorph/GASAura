// Luis Correa


#include "AI/Services/BTService_FindNearestPlayer.h"

#include "NiagaraValidationRule.h"
#include "Kismet/KismetSystemLibrary.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/*
	AActor * OwnerActor = OwnerComp.GetOwner();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);

	TArray<AActor*> OutActors;

	float NearestDistance = 9999999999;

	AActor * NearestActor;
	
	if(UKismetSystemLibrary::SphereOverlapActors(OwnerActor,OwnerActor->GetActorLocation(), 5000, ObjectTypes, APawn::StaticClass(), ActorsToIgnore, OutActors))
	{
		for (auto OverlappedActor : OutActors)
		{
			if(OverlappedActor->ActorHasTag("Player"))
			{
				GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, OverlappedActor->GetName());

				float DistanceToTarget =  FVector::Distance(OwnerActor->GetActorLocation(), OverlappedActor->GetActorLocation());

				if(DistanceToTarget < NearestDistance)
					NearestActor = OverlappedActor;
				
			}
		}
		
	}

	//TargetToFollowSelector
	//DistanceToTargetSelector
	*/
}
