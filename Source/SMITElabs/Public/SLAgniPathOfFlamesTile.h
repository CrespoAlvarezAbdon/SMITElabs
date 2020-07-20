// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMITElabs/Public/SLGod.h"
#include "SMITElabs/Public/SLAgni.h"
#include "SLAgniPathOfFlamesTile.generated.h"

class ASLGod;
class ASLAgni;

UCLASS()
class SMITELABS_API ASLAgniPathOfFlamesTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLAgniPathOfFlamesTile();

	void SetTileMaterial(UMaterial* Val);

	void SetOrigin(AActor* Val);

	AActor* GetOrigin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	AActor* Origin;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};