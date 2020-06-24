// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SLGod.h"
#include "SLAgni.generated.h"

/**
 * 
 */
UCLASS()
class SMITELABS_API ASLAgni : public ASLGod
{
	GENERATED_BODY()
	
public:
	ASLAgni();

protected:

	FTimerDelegate RainFireTimerDelegate;

	int CombustionCount{ 0 };

	int UltCharges{ 0 };

	virtual void OnBasicAttackHit(TArray<ISLVulnerable*> Targets) override;

	virtual void UseAbility1() override;

	virtual void UseAbility2() override;

	virtual void UseAbility3() override;

	virtual void UseAbility4() override;

	virtual void LevelAbility4() override;

	void ConsumeCombustionStacks();

	void CheckRainFireTimer();

	UFUNCTION()
	void AddUltCharge();
};
