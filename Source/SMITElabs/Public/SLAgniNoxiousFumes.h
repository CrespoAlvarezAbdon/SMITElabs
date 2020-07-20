// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMITElabs/Public/SLGod.h"
#include "SMITElabs/Public/SLAgni.h"
#include "SMITElabs/Public/SLAgniFlameWave.h"
#include "SMITElabs/Public/SLAgniPathOfFlamesTile.h"
#include "SMITElabs/Public/SLAgniRainFire.h"
#include "SLAgniNoxiousFumes.generated.h"

class ISLVulnerable;
class ASLAgniFlameWave;
class ASLAgniPathOfFlamesTile;
class ASLAgniRainFire;

UCLASS()
class SMITELABS_API ASLAgniNoxiousFumes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLAgniNoxiousFumes();

	void SetOrigin(ASLGod* Val);

	void SetDamage(float Val);

	void SetScaling(float Val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MNoxiousFumesActive;

	FTimerHandle DestroyFumesTimerHandle;

	FTimerHandle DealFumesDamageTimerHandle;

	ASLGod* Origin;

	float Damage;

	float Scaling;

	UFUNCTION()
	void DealFumesDamage();

	UFUNCTION()
	void DestroyFumes();

	void ActivateStun();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

};
