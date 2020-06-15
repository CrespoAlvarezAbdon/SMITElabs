// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLMobile.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLMobile : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITELABS_API ISLMobile
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	void SetMovementSpeed(float Val);

protected:

	float BaseMovementSpeed{ 360 };

	float UndiminishedMovementSpeed{ 0 };

	float DiminishedMovementSpeed{ 0 };

	const float MovementSpeedDiminishments[2]{ 457, 540.5 };

	const float MovementSpeedDiminishmentMultipliers[2]{ 0.8, 0.5 };

	const float MinimumMovementSpeed{ 150 };

	const float MaximumDiminishedMovementSpeed{ 753.55 };
};
