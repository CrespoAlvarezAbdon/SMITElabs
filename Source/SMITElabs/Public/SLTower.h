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

	virtual void TakeHealthDamage(float Val, ISLDangerous* Origin) override;

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

	float TowerDamageMultiplier{ 1 };
	float TowerDamageMultiplierIncrement{ 0.2 };

	float TowerHealthPerSecond{ 8 };

	float TowerMaxRegenHealth{ 0 };

	bool bHasShaft{ true };

	bool bHasTowerHPS{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP Variables")
	bool bIsOrderBP{ true };

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
