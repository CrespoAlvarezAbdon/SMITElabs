// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMITElabs/Public/SLGod.h"
#include "SMITElabs/Public/SLAgni.h"
#include "SLAgniRainFire.generated.h"

class ASLGod;
class ASLAgni;

UCLASS()
class SMITELABS_API ASLAgniRainFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLAgniRainFire();

	void SetOrigin(ASLGod* Val);

	void SetDamage(float Val);

	void SetScaling(float Val);

	void SetBHasCombustion(bool Val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeteorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* DamageAreaComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MRainFireExplosion;

	ASLGod* Origin;

	FTimerHandle MeteorMovementTimerHandle;

	FVector StartingLocation;

	TArray<ASLGod*> HitGods;

	bool bHasCombustion;

	float Damage;

	float Scaling;

	float DirectionalMultiplier{ 1 };

	void AdjustMeteor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
