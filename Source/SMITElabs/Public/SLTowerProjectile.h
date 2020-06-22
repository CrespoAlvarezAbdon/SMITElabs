// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "SLTowerProjectile.generated.h"

class UProjectileMovementComponent;
class UCapsuleComponent;
class ASLTower;
class ISLVulnerable;

UCLASS()
class SMITELABS_API ASLTowerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLTowerProjectile();

	void SetCurrentTarget(ISLVulnerable* Val);

	void SetOrigin(ASLTower* Val);

	void SetTowerDamageMultiplier(float Val);

	void SetProjectileSpeed(float Val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	ISLVulnerable* CurrentTarget;

	ASLTower* Origin;

	float TowerDamageMultiplier;

	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
