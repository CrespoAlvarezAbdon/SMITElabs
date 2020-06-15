// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLDangerous.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLDangerous : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITELABS_API ISLDangerous
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	void SetBasicAttackSpeed(float Val);

	float GetCurrentBasicAttackDamage() const;

	float GetPhysicalPower() const;

	float GetMagicalPower() const;

	float GetFlatPhysicalPenetration() const;

	float GetFlatMagicalPenetration() const;

	float GetPercentagePhysicalPenetration() const;

	float GetPercentageMagicalPenetration() const;

	float GetBasicAttackPowerScaling() const;

	bool GetIsPhysicalDamage() const;

protected:

	float BasicAttackSpeed{ 1 };

	bool bIsBasicAttacking{ false };

	bool bFatalis{ false };

	float BasicAttackPenalty{ 1 };

	float BaseBasicAttackDamage{ 43 };

	float CurrentBasicAttackDamage{ BaseBasicAttackDamage };

	float BasicAttackPowerScaling{ 1 };

	float PhysicalPower{ 0 };

	float MagicalPower{ 0 };

	float FlatPhysicalPenetration{ 0 };

	float FlatMagicalPenetration{ 0 };

	float PercentagePhysicalPenetration{ 0 };

	float PercentageMagicalPenetration{ 0 };

	bool bIsPhysicalDamage{ true };

	TArray<float> BasicAttackRefireProgression{ 0.5, 1, 1 };

	TArray<float> BasicAttackPrefireProgression{ 0.25, 0.25, 0.5 };

	TArray<float> BasicAttackRangeProgression{ 55, 55, 55 };

	TArray<float> BasicAttackDamageProgression{ BasicAttackRefireProgression };

	TArray<float> BasicAttackDisjointProgression{ 306.25, 0, 306.25, 0, 306.25, 0 };

	TArray<float> RangedBasicAttackProjectileSizeProgression{ 3, 3, 3 };

	TArray<float> RangedBasicAttackProjectileSpeedProgression{ 110, 110, 110 };

	TArray<bool> bCleaveProgression{ false, false, false };

	TArray<float> CleaveDamageProgression{ 1, 1, 1 };

	TArray<float> RangedCleaveRangeProgression{ 7.5, 7.5, 7.5 };

	TArray<bool> bIsBasicAttackRangedProgression{ true, true, true };

	TArray<bool> bHasScalingPrefireProgression{ true, true, true };

	int CurrentProgression{ 0 };

	const float ProgressionResetTime{ 1 };

	float BasicAttackStrafePenalty{ 0.8 };

	float BasicAttackBackpedalPenalty{ 0.6 };

	float BasicAttackRangedPenalty{ 0.5 };

	float BasicAttackMeleePenalty{ 0.65 };

};
