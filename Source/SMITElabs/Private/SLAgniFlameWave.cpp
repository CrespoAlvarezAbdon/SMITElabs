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

	DestroyWaveTimerDelegate.BindUFunction(this, FName("DestroyWave"), false);
}

void ASLAgniFlameWave::SetOrigin(ASLGod* Val) { Origin = Val; }

void ASLAgniFlameWave::SetDamage(float Val) { Damage = Val; }

void ASLAgniFlameWave::SetScaling(float Val) { Scaling = Val; }

void ASLAgniFlameWave::DestroyWave() { Destroy(); }

// Called when the game starts or when spawned
void ASLAgniFlameWave::BeginPlay()
{
	Super::BeginPlay();

	StartingLocation = GetActorLocation();
}

// Called every frame
void ASLAgniFlameWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVector::Dist(StartingLocation, GetActorLocation() + SceneComponent->GetForwardVector() * ProjectileSpeed * 100 * DeltaTime) >= (ProjectileRange - StaticMeshComponent->GetRelativeScale3D().X) * 100)
	{
		SetActorLocation(StartingLocation + SceneComponent->GetForwardVector() * (ProjectileRange - StaticMeshComponent->GetRelativeScale3D().X) * 100);
		GetWorld()->GetTimerManager().SetTimer(DestroyWaveTimerHandle, DestroyWaveTimerDelegate, .5, false);
		SetActorTickEnabled(false);
	}
	SetActorLocation(GetActorLocation() + SceneComponent->GetForwardVector() * ProjectileSpeed * 100 * DeltaTime);
}

