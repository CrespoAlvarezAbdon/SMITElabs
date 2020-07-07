// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAgniFlameWave.h"

// Sets default values
ASLAgniFlameWave::ASLAgniFlameWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void ASLAgniFlameWave::SetOrigin(ASLGod* Val) { Origin = Val; }

void ASLAgniFlameWave::SetDamage(float Val) { Damage = Val; }

void ASLAgniFlameWave::SetScaling(float Val) { Scaling = Val; }

// Called when the game starts or when spawned
void ASLAgniFlameWave::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASLAgniFlameWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

