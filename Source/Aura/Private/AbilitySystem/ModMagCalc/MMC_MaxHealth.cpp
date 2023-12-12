// Luis Correa


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/AuraCombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//gather tags from src and target

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluatedParams;
	EvaluatedParams.SourceTags = SourceTags;
	EvaluatedParams.TargetTags = TargetTags;

	float VigorMagnitude;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluatedParams, VigorMagnitude);
	VigorMagnitude = FMath::Max(VigorMagnitude, 0);

	IAuraCombatInterface * SourceCharacter = Cast<IAuraCombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = SourceCharacter->GetPlayerLevel();

	return 80.f + (2.5f *VigorMagnitude) + (10.f * PlayerLevel);
}
