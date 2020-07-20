// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAgniRainFire.h"

// Sets default values
ASLAgniRainFire::ASLAgniRainFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	MeteorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeteorComponent"));
	MeteorComponent->SetupAttachment(RootComponent);
	DamageAreaComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DamageAreaComponent"));
	DamageAreaComponent->SetupAttachment(RootComponent);
}

void ASLAgniRainFire::SetOrigin(ASLGod* Val) { Origin = Val; }

void ASLAgniRainFire::SetDamage(float Val) { Damage = Val; }

void ASLAgniRainFire::SetScaling(float Val) { Scaling = Val; }

void ASLAgniRainFire::SetBHasCombustion(bool Val) { bHasCombustion = Val; }

// Called when the game starts or when spawned
void ASLAgniRainFire::BeginPlay()
{
	Super::BeginPlay();

	MeteorComponent->SetWorldLocation(GetActorLocation() + SceneComponent->GetUpVector() * 300);
	DamageAreaComponent->SetWorldLocation(Origin->GetTargeterLocationComponent()->GetComponentLocation());

	StartingLocation = MeteorComponent->GetComponentLocation();
	GetWorldTimerManager().SetTimer(MeteorMovementTimerHandle, this, &ASLAgniRainFire::AdjustMeteor, .8, true);
}

void ASLAgniRainFire::AdjustMeteor()
{
	if (DirectionalMultiplier >= 0)
	{
		MeteorComponent->SetWorldLocation(DamageAreaComponent->GetComponentLocation() + SceneComponent->GetUpVector() * .8 * 10000 * DirectionalMultiplier);
		MeteorComponent->SetRelativeScale3D(FVector(10, 10, 10));
		float TimeRemaining{ GetWorldTimerManager().GetTimerElapsed(MeteorMovementTimerHandle) };
		GetWorldTimerManager().ClearTimer(MeteorMovementTimerHandle);
		GetWorldTimerManager().SetTimer(MeteorMovementTimerHandle, this, &ASLAgniRainFire::AdjustMeteor, .4 + .8 - TimeRemaining, true);
		StartingLocation = MeteorComponent->GetComponentLocation();
		DirectionalMultiplier = -2.2;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(MeteorMovementTimerHandle);
		SetActorTickEnabled(false);
		MeteorComponent->SetVisibility(false);
		DamageAreaComponent->SetMaterial(0, MRainFireExplosion);
		DamageAreaComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		TArray<AActor*> OverlappingActors;
		DamageAreaComponent->GetOverlappingActors(OverlappingActors);
		for (AActor* OverlappingActor : OverlappingActors)
		{
			if (Cast<ASLGod>(OverlappingActor) && !HitGods.Contains(Cast<ASLGod>(OverlappingActor)) && OverlappingActor != Origin)
			{
				HitGods.Add(Cast<ASLGod>(OverlappingActor));
				Cast<ASLGod>(OverlappingActor)->TakeHealthDamage((Damage + Origin->GetMagicalPower() * Scaling) * (100 / (Origin->CalculateTotalProtections(Cast<ASLGod>(OverlappingActor)) + 100)), Origin);
				if (bHasCombustion) Cast<ASLAgni>(Origin)->AddCombustionTarget(Cast<ASLGod>(OverlappingActor));
			}
		}
		SetLifeSpan(.5);
	}
}

// Called every frame
void ASLAgniRainFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MeteorComponent->SetWorldLocation(StartingLocation + SceneComponent->GetUpVector() * GetWorldTimerManager().GetTimerElapsed(MeteorMovementTimerHandle) * 10000 * DirectionalMultiplier);
}

