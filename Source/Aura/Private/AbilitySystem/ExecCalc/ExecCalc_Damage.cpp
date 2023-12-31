// Luis Correa


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplaytags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/AuraCombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistence);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightingResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagToAttributeDefinition;
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistence, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightingResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);

		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		
		TagToAttributeDefinition.Add(GameplayTags.Attributes_Secondary_Armor, ArmorDef);
		TagToAttributeDefinition.Add(GameplayTags.Attributes_Primary_ArmorPenetration, ArmorPenetrationDef);
		TagToAttributeDefinition.Add(GameplayTags.Attributes_Primary_BlockChance, BlockChanceDef);
		TagToAttributeDefinition.Add(GameplayTags.Attributes_Primary_CriticalHitChance, CriticalHitChanceDef);
		TagToAttributeDefinition.Add(GameplayTags.Attributes_Primary_CriticalHitDamage, CriticalHitDamageDef);
		TagToAttributeDefinition.Add(GameplayTags.Attributes_Primary_CriticalHitResistence, CriticalHitResistenceDef);

		TagToAttributeDefinition.Add(GameplayTags.Attributes_Resistance_Fire, FireResistanceDef);
		TagToAttributeDefinition.Add(GameplayTags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagToAttributeDefinition.Add(GameplayTags.Attributes_Resistance_Lighting, LightingResistanceDef);
		TagToAttributeDefinition.Add(GameplayTags.Attributes_Resistance_Physical, PhysicalResistanceDef);

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
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistenceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightingResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent * SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent * TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor * SourceAvatar = nullptr;
	AActor * TargetAvatar = nullptr;

	if(SourceASC)
		SourceAvatar = SourceASC ->GetAvatarActor();

	if(TargetASC)
		TargetAvatar = TargetASC->GetAvatarActor();

	IAuraCombatInterface* SourceCharacterCombatInterface = Cast<IAuraCombatInterface>(SourceAvatar);
	IAuraCombatInterface* TargetCharacterCombatInterface = Cast<IAuraCombatInterface>(TargetAvatar);


	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluatedParams;
	EvaluatedParams.SourceTags = SourceTags;
	EvaluatedParams.TargetTags = TargetTags;

	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	//Get damage set by caller magnitude
	float Damage = 0;
	
	for(auto& Pair : FAuraGameplayTags::Get().DamageTypeToResistance)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);

		/*
		check(DamageStatics().TagToAttributeDefinition.Contains(ResistanceTag));
		
		const FGameplayEffectAttributeCaptureDefinition ResAttribCaptureDef = DamageStatics().TagToAttributeDefinition[ResistanceTag];
		float ResistanceValue = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResAttribCaptureDef, EvaluatedParams, ResistanceValue);
		ResistanceValue = FMath::Clamp(ResistanceValue, 0.0f,100.0f);
		
		DamageTypeValue *= (100.f - ResistanceValue) / 100.f;
		*/
		
		Damage += DamageTypeValue;
	}
	
	//capture block chance
	float BlockChanceTarget;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluatedParams, BlockChanceTarget);

	const float Rand = FMath::RandRange(0.0f, 100.0f);
	const bool bBlocked = Rand <= BlockChanceTarget;

	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetBlockedHit(ContextHandle, bBlocked);
	
	if(bBlocked)
		Damage = Damage * 0.5f;

	float ArmorTarget;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluatedParams, ArmorTarget);

	float ArmorPenetrationSource;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluatedParams, ArmorPenetrationSource);

	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	
	float ArmorPenetrationCoef = 0.25f;
	float EffectiveArmorCoef = 0.333f;

	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalcCoef.Get()->FindCurve(FName("ArmorPenetration"), FString());
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalcCoef.Get()->FindCurve(FName("EffectiveArmor"), FString());
	
	if(SourceCharacterCombatInterface)
		ArmorPenetrationCoef = ArmorPenetrationCurve->Eval(SourceCharacterCombatInterface->GetPlayerLevel());

	if(TargetCharacterCombatInterface)
		EffectiveArmorCoef = EffectiveArmorCurve->Eval(TargetCharacterCombatInterface->GetPlayerLevel());
	
	const float EffectiveArmor = ArmorTarget * (100.0f - ArmorPenetrationSource * ArmorPenetrationCoef) / 100.0f;

	Damage *= (100 - EffectiveArmor * EffectiveArmorCoef) / 100;

	float CriticalHitChanceSource;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluatedParams, CriticalHitChanceSource);

	float CriticalHitResistenceTarget;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistenceDef, EvaluatedParams, CriticalHitResistenceTarget);

	const float EffectiveCritHitChance = CriticalHitChanceSource - CriticalHitResistenceTarget * 0.15f;
	
	bool Crit = FMath::RandRange(0.0f, 100.0f) <= EffectiveCritHitChance;
	UAuraAbilitySystemLibrary::SetCriticalHit(ContextHandle, Crit);
	
	if(Crit)
	{
		float CriticalHitDamageSource;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluatedParams, CriticalHitDamageSource);

		Damage *= 2;
		Damage += CriticalHitDamageSource;
	}
	
	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
