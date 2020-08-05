// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SLGod.h"
#include "SLAgni.generated.h"

class ASLAgniNoxiousFumes;
class ASLAgniFlameWave;
class ASLAgniPathOfFlamesTile;
class ASLAgniRainFire;

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

	TArray<ASLGod*> GetPathOfFlamesTargets();

	void AddCombustionTarget(ASLGod* CombustionTarget);

	void AddPathOfFlamesTarget(ASLGod* PathOfFlamesTarget);

	virtual void SetGodLevel(int Val) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* HaloOriginComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> HaloMeteorComponents;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MAgni;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MAgniCombustionStacked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MPathOfFlamesLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MPathOfFlamesDark;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<class ASLAgniNoxiousFumes> NoxiousFumesTile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<class ASLAgniFlameWave> FlameWaveProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<class ASLAgniPathOfFlamesTile> PathOfFlamesTile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<class ASLAgniRainFire> RainFireTile;

	FTimerHandle NoxiousFumesPrefireTimerHandle;

	FTimerHandle FlameWavePrefireTimerHandle;

	FTimerDelegate FlameWavePrefireTimerDelegate;

	TArray<FTimerHandle> PathOfFlamesTimerHandles;

	TArray<FTimerDelegate> PathOfFlamesTimerDelegates;

	TArray<ASLGod*> PathOfFlamesTargets;

	TArray<int> PathOfFlamesTicks;

	FTimerHandle EndRainFireTimerHandle;

	TArray<FTimerHandle> CombustionTimerHandles;

	TArray<FTimerDelegate> CombustionTimerDelegates;

	TArray<ASLGod*> CombustionTargets;

	TArray<int> CombustionTicks;

	int CombustionCount{ 0 };

	float CombustionDamage{ 5 };

	float CombustionScaling{ .1 };

	ASLAgniNoxiousFumes* NoxiousFumes;

	FTransform NoxiousFumesSpawnTransform;

	TArray<float> NoxiousFumesDamage = { 10, 20, 30, 40, 50 };

	float NoxiousFumesScaling{ .05 };

	TArray<float> FlameWaveDamage = {90, 140, 190, 240, 290};

	float FlameWaveScaling{ .65 };

	bool bIsUsingPathOfFlames{ false };

	float PathOfFlamesSpawnDistance{ 0 };

	float PathOfFlamesDistanceTravelled{ 0 };

	TArray<float> PathOfFlamesDamage = { 20, 30, 40, 50, 60 };

	float PathOfFlamesScaling{ .15 };

	TArray<float> RainFireDamage = { 140, 180, 220, 260, 300 };

	float RainFireScaling{ .7 };

	void UseNoxiousFumes();

	UFUNCTION()
	void UseFlameWave();

	void UsePathOfFlames();

	void EndPathOfFlames();

	void UseRainFire();

	void EndRainFire();

	virtual void OnBasicAttackHit(TArray<ISLVulnerable*> Targets) override;

	virtual void FireAbility(int AbilitySlot) override;

	void ConsumeCombustionStacks();

	UFUNCTION()
	void DealCombustionDamage(ASLGod* CombustionTarget, FTimerHandle CombustionTimerHandle, int& CombustionTick);

	UFUNCTION()
	void DealPathOfFlamesDamage(ASLGod* PathOfFlamesTarget, FTimerHandle PathOfFlamesTimerHandle, int& PathOfFlamesTick);

	virtual void OnAbilityCooldownEnded(int AbilityID) override;

	virtual void LevelAbility(int AbilitySlot) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
