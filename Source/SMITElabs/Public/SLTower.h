// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SMITElabs/Public/SLVulnerable.h"
#include "SMITElabs/Public/SLDangerous.h"
#include "SMITElabs/Public/SLIdentifiable.h"
#include "SLTower.generated.h"

class UStaticMeshComponent;
class ISLVulnerable;
class ISLDangerous;
class ISLIdentifiable;
class UGameplayStatics;

UCLASS()
class SMITELABS_API ASLTower : public AActor, public ISLVulnerable, public ISLDangerous, public ISLIdentifiable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLTower();

	virtual bool GetBIsOrder() const override;

	virtual float GetPhysicalProtections() const override;

	virtual float GetMagicalProtections() const override;

	virtual void TakeHealthDamage(float Val, ISLDangerous* Origin) override;

	virtual void SetBasicAttackSpeed(float Val) override;

	virtual float GetCurrentBasicAttackDamage() const override;

	virtual float GetPhysicalPower() const override;

	virtual float GetMagicalPower() const override;

	virtual float GetFlatPhysicalPenetration() const override;

	virtual float GetFlatMagicalPenetration() const override;

	virtual float GetPercentagePhysicalPenetration() const override;

	virtual float GetPercentageMagicalPenetration() const override;

	virtual float GetBasicAttackPowerScaling() const override;

	virtual bool GetIsPhysicalDamage() const override;

	virtual void OnBasicAttackHit(TArray<ISLVulnerable*> Targets) override;

	virtual float CalculateTotalProtections(ISLVulnerable* Targeted) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TowerBaseComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TowerShaftComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TowerHeadComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	UStaticMeshComponent* TowerShotRangeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ASLTowerProjectile> TowerProjectile;

	bool bHasShaft{ true };

	bool bHasTowerHPS{ false };

#pragma region Offense

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offense")
	float AttackInterval{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offense")
	float AttackDelay{ .25 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offense")
	float BaseBasicAttackDamage{ 215 };

	float CurrentBasicAttackDamage{ BaseBasicAttackDamage };

	float TowerDamageMultiplier{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offense")
	float TowerDamageMultiplierIncrement{ 0.2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offense")
	float ProjectileSpeed{ 7500 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offense")
	bool bIsPhysicalDamage{ true };
	
	// Power and pen is largely vestigial
	float PhysicalPower{ 0 };
	float MagicalPower{ 0 };
	float BasicAttackPowerScaling{ 1 };
	float FlatPhysicalPenetration{ 0 };
	float FlatMagicalPenetration{ 0 };
	const float MaxFlatPenetration{ 50 };
	float PercentagePhysicalPenetration{ 0 };
	float PercentageMagicalPenetration{ 0 };
	const float MaxPercentagePenetration{ .4 };

#pragma endregion

#pragma region Defense

	float PhysicalProtections{ 125 };

	float MagicalProtections{ 125 };

	float BaseHealth{ 2500 };

	float CurrentHealth{ BaseHealth };

	float MaxHealth{ BaseHealth };

	float Shield{ 0 };

	const float MaxProtection{ 325 };

	bool bHasBasicHealthBar{ false };

	int NumberOfBasics{ 0 };

	float TowerMaxRegenHealth{ BaseHealth * .7f };

	float TowerHealthPerSecond{ 8 };

#pragma endregion

#pragma region Identity

	FString UnitName{ "Unknown" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	bool bIsOrder{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	bool bIsNeutral{ false };

#pragma endregion

	ISLVulnerable* CurrentTarget{ nullptr };

	ISLVulnerable* PriorityTarget{ nullptr };

	ISLVulnerable* PreviousTarget{ nullptr };

	ASLTowerProjectile* SpawnedTowerProjectile;

	FTimerHandle FireTowerShotTimerHandle;

	FTimerDelegate PrepareTowerShotTimerDelegate;

	FTimerDelegate FireTowerShotTimerDelegate;

	FTimerDelegate BeginPlayTimerDelegate;

	FTimerHandle TowerHealthRegenTimerHandle;

	FTimerDelegate TowerHealthRegenTimerDelegate;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SearchForTarget(bool bSetTimer);

	UFUNCTION()
	void InitialSearchForTarget();

	UFUNCTION()
	void PrepareTowerShot();

	UFUNCTION()
	void FireTowerShot();

	UFUNCTION()
	void HealTower();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
