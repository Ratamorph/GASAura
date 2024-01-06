// Luis Correa

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraEnemyAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AAuraEnemyAIController();

protected:
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
};
