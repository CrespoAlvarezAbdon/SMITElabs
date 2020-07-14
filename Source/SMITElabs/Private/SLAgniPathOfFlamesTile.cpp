// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAgniPathOfFlamesTile.h"

// Sets default values
ASLAgniPathOfFlamesTile::ASLAgniPathOfFlamesTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void ASLAgniPathOfFlamesTile::SetTileMaterial(UMaterial* Val) { StaticMeshComponent->SetMaterial(0, Val); }

// Called when the game starts or when spawned
void ASLAgniPathOfFlamesTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASLAgniPathOfFlamesTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

