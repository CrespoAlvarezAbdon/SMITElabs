// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SMITElabs/Public/SLVulnerable.h"
#include "SLDangerous.generated.h"

class ISLVulnerable;

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

	virtual void SetBasicAttackSpeed(float Val) = 0;

	virtual float GetCurrentBasicAttackDamage() const = 0;

	virtual float GetPhysicalPower() const = 0;

	virtual float GetMagicalPower() const = 0;

	virtual float GetFlatPhysicalPenetration() const = 0;

	virtual float GetFlatMagicalPenetration() const = 0;

	virtual float GetPercentagePhysicalPenetration() const = 0;

	virtual float GetPercentageMagicalPenetration() const = 0;

	virtual float GetBasicAttackPowerScaling() const = 0;

	virtual bool GetIsPhysicalDamage() const = 0;

	virtual float CalculateTotalProtections(ISLVulnerable* Targeted) const = 0;

protected:

	

};
