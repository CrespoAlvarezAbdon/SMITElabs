// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SMITElabs/Public/SLGod.h"
#include "SMITElabs/Public/SLAgni.h"
#include "SLAgniFlameWave.generated.h"

class ASLGod;
class ASLAgni;
class UProjectileMovementComponent;

UCLASS()
class SMITELABS_API ASLAgniFlameWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLAgniFlameWave();

	void SetOrigin(ASLGod* Val);

	void SetDamage(float Val);

	void SetScaling(float Val);

	void SetBHasCombustion(bool Val);

protected:
	// Called when the game starts or when spawned
	UFUNCTION()
	void DestroyWave();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	FTimerHandle DestroyWaveTimerHandle;

	FTimerDelegate DestroyWaveTimerDelegate;

	ASLGod* Origin;

	TArray<ASLGod*> HitGods;

	FVector StartingLocation;

	bool bHasCombustion;

	float Damage;

	float Scaling;

	float ProjectileSpeed{ 150 };

	float ProjectileRange{ 50 };

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
