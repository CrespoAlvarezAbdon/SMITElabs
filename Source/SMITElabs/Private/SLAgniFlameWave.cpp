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

	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLAgniFlameWave::OnOverlapBegin);
}

void ASLAgniFlameWave::SetOrigin(ASLGod* Val) { Origin = Val; }

void ASLAgniFlameWave::SetDamage(float Val) { Damage = Val; }

void ASLAgniFlameWave::SetScaling(float Val) { Scaling = Val; }

void ASLAgniFlameWave::SetBHasCombustion(bool Val) { bHasCombustion = Val; }

void ASLAgniFlameWave::DestroyWave() { Destroy(); }

// Called when the game starts or when spawned
void ASLAgniFlameWave::BeginPlay()
{
	Super::BeginPlay();

	StartingLocation = GetActorLocation();
}

void ASLAgniFlameWave::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (Cast<ASLGod>(OtherActor) && !HitGods.Contains(Cast<ASLGod>(OtherActor)) && OtherActor != Origin)
	{
		HitGods.Add(Cast<ASLGod>(OtherActor));
		Cast<ASLGod>(OtherActor)->TakeHealthDamage((Damage + Origin->GetMagicalPower() * Scaling) * (100 / (Origin->CalculateTotalProtections(Cast<ASLGod>(OtherActor)) + 100)), Origin);
		if (bHasCombustion && !Cast<ASLAgni>(Origin)->GetCombustionTargets().Contains(Cast<ASLGod>(OtherActor))) Cast<ASLAgni>(Origin)->AddCombustionTarget(Cast<ASLGod>(OtherActor));
	}
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
	else SetActorLocation(GetActorLocation() + SceneComponent->GetForwardVector() * ProjectileSpeed * 100 * DeltaTime);
}

