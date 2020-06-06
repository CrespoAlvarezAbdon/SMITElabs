// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "SLRangedBasicProjectile.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class UBoxComponent;
class UArrowComponent;
class UPrimitiveComponent;
class AActor;
class ASLGod;

UCLASS()
class SMITELABS_API ASLRangedBasicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLRangedBasicProjectile();

	void SetBasicAttackDisjoints(TArray<float> Val);

	void SetOrigin(ASLGod* Val);

	void SetDamageProgressionMultiplier(float Val);

	void SetProjectileSpeed(float Val);

	void SetProjectileRange(float Val);

	void SetProjectileSize(float Val);

	void SetCleave(bool Val);

	void SetCleaveDamage(float Val);

	void SetCleaveRange(float Val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* CleaveCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	UBoxComponent* WallCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arrow")
	UArrowComponent* ArrowComponent;

	AActor* Origin;

	FVector StartingLocation;

	float BasicAttackDisjoints[2];

	float DamageProgressionMultiplier;

	float ProjectileSpeed;

	float ProjectileRange;

	float ProjectileSize;

	const float ProjectileLength{ 3 };

	const float ProjectileHeight{ 1 };

	bool bCleave;

	float CleaveDamage;

	float CleaveRange;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnWallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};