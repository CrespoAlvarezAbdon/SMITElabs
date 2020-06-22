// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLVulnerable.generated.h"

class ISLDangerous;

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

	virtual void TakeHealthDamage(float Val, ISLDangerous* Origin) = 0;

	virtual float GetPhysicalProtections() const = 0;

	virtual float GetMagicalProtections() const = 0;

};
