// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SLGod.h"
#include "SLAgni.generated.h"

class ASLAgniFlameWave;
class ASLAgniPathOfFlamesTile;

/**
 * 
 */
UCLASS()
class SMITELABS_API ASLAgni : public ASLGod
{
	GENERATED_BODY()
	
public:
	ASLAgni();

	TArray<ASLGod*> GetCombustionTargets();

	void AddCombustionTarget(ASLGod* CombustionTarget);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MPathOfFlamesLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MPathOfFlamesDark;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<class ASLAgniFlameWave> FlameWaveProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<class ASLAgniPathOfFlamesTile> PathOfFlamesTile;

	FTimerHandle FlameWavePrefireTimerHandle;

	FTimerDelegate FlameWavePrefireTimerDelegate;

	FTimerHandle PathOfFlamesTimerHandle;

	FTimerDelegate PathOfFlamesTimerDelegate;

	TArray<FTimerHandle> CombustionTimerHandles;

	TArray<FTimerDelegate> CombustionTimerDelegates;

	TArray<ASLGod*> CombustionTargets;

	TArray<int> CombustionTicks;

	int CombustionCount{ 0 };

	float CombustionDamage{ 5 };

	float CombustionScaling{ .1 };

	TArray<float> FlameWaveDamage = {90, 140, 190, 240, 290};

	float FlameWaveScaling{ .65 };

	bool bIsUsingPathOfFlames{ false };

	float PathOfFlamesSpawnDistance{ 0 };

	float PathOfFlamesDistanceTravelled{ 0 };

	FVector PathOfFlamesPreviousLocation;

	UFUNCTION()
	void UseFlameWave();

	void UsePathOfFlames();

	UFUNCTION()
	void SpawnPathOfFlamesTile();

	void EndPathOfFlames();

	virtual void OnBasicAttackHit(TArray<ISLVulnerable*> Targets) override;

	virtual void FireAbility(int AbilitySlot) override;

	void ConsumeCombustionStacks();

	UFUNCTION()
	void DealCombustionDamage(ASLGod* CombustionTarget, FTimerHandle CombustionTimerHandle, int& CombustionTick);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
