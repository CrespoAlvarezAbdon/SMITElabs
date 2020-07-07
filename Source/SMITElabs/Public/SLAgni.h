// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SLGod.h"
#include "SLAgni.generated.h"

class ASLAgniFlameWave;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<class ASLAgniFlameWave> FlameWaveProjectile;

	FTimerHandle FlameWaveDelayTimerHandle;

	FTimerDelegate FlameWaveDelayTimerDelegate;

	int CombustionCount{ 0 };

	float CombustionDamage{ 5 };

	float CombustionScaling{ .1 };

	TArray<float> FlameWaveDamage = {90, 140, 190, 240, 290};

	float FlameWaveScaling{ .65 };

	UFUNCTION()
	void UseFlameWave();

	virtual void OnBasicAttackHit(TArray<ISLVulnerable*> Targets) override;

	virtual void FireAbility(int AbilitySlot) override;

	void ConsumeCombustionStacks();
};
