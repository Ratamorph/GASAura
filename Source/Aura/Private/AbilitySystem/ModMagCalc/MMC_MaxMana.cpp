// Luis Correa


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/AuraCombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//gather tags from src and target

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluatedParams;
	EvaluatedParams.SourceTags = SourceTags;
	EvaluatedParams.TargetTags = TargetTags;

	float IntelligenceMagnitude;
	GetCapturedAttributeMagnitude(IntelligenceDef,Spec,EvaluatedParams, IntelligenceMagnitude);
	IntelligenceMagnitude = FMath::Max(IntelligenceMagnitude, 0);

	IAuraCombatInterface * SourceCharacter = Cast<IAuraCombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = SourceCharacter->GetPlayerLevel();

	return 50.f + (2.5f *IntelligenceMagnitude) + (15.f * PlayerLevel);
}
