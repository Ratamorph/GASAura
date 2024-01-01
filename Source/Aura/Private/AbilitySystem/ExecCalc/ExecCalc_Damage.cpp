// Luis Correa


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplaytags.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);

	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent * SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent * TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor * SourceAvatar = nullptr;
	const AActor * TargetAvatar = nullptr;

	if(SourceASC)
		SourceAvatar = SourceASC ->GetAvatarActor();

	if(TargetASC)
		TargetAvatar = TargetASC->GetAvatarActor();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluatedParams;
	EvaluatedParams.SourceTags = SourceTags;
	EvaluatedParams.TargetTags = TargetTags;

	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	//Get damage set by caller magnitude
	float Damage = Spec.GetSetByCallerMagnitude(GameplayTags.Damage);

	//capture block chance
	float BlockChanceTarget;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluatedParams, BlockChanceTarget);

	const float Rand = FMath::RandRange(0.0f, 100.0f);
	const bool bBlocked = Rand <= BlockChanceTarget;

	if(bBlocked)
		Damage = Damage * 0.5f;

	float ArmorTarget;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluatedParams, ArmorTarget);

	float ArmorPenetrationSource;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluatedParams, ArmorPenetrationSource);

	const float EffectiveArmor = ArmorTarget * (100.0f - ArmorPenetrationSource * 0.25f) / 100.0f;

	Damage *= (100 - EffectiveArmor * 0.333f) / 100;
	
	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
