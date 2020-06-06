// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLDamageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITELABS_API ISLDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetCurrentHealth(float Val, AActor* Origin);

	virtual float GetPhysicalProtections();

	virtual float GetMagicalProtections();

protected:

	float BaseHealth{ 600 };

	float CurrentHealth{ BaseHealth };

	float MaxHealth{ BaseHealth };

	float Shield{ 0 };

	float PhysicalProtections{ 100 };

	float MagicalProtections{ 100 };

	bool bHasBasicHealthBar{ false };

	int NumberOfBasics{ 0 };
};
