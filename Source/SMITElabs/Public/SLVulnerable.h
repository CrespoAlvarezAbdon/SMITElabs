// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLVulnerable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLVulnerable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITELABS_API ISLVulnerable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void TakeHealthDamage(float Val, AActor* Origin) PURE_VIRTUAL(ISLVulnerable::TakeHealthDamage);

	virtual float GetPhysicalProtections();

	virtual float GetMagicalProtections();

protected:

	float BaseHealth{ 600 };

	float CurrentHealth{ BaseHealth };

	float MaxHealth{ BaseHealth };

	float Shield{ 0 };

	float PhysicalProtections{ 100 };

	float MagicalProtections{ 100 };

	const float MaxProtection{ 325 };

	const float MaxPenetration{ 50 };

	bool bHasBasicHealthBar{ false };

	int NumberOfBasics{ 0 };
};
