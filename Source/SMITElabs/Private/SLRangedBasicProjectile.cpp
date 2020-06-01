// Fill out your copyright notice in the Description page of Project Settings.


#include "SLRangedBasicProjectile.h"
#include "SMITElabs/Public/SLGod.h"

// Sets default values
ASLRangedBasicProjectile::ASLRangedBasicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLRangedBasicProjectile::OnOverlapBegin);
}

void ASLRangedBasicProjectile::SetBasicAttackDisjoints(TArray<float> Val)
{
	BasicAttackDisjoints[0] = Val[0];
	BasicAttackDisjoints[1] = Val[1];
}

void ASLRangedBasicProjectile::SetOrigin(ASLGod* Val) { Origin = Val; }

void ASLRangedBasicProjectile::SetDamageProgressionMultiplier(float Val) { DamageProgressionMultiplier = Val; }

void ASLRangedBasicProjectile::SetProjectileSpeed(float Val) { ProjectileSpeed = Val; }

void ASLRangedBasicProjectile::SetProjectileRange(float Val) { ProjectileRange = Val; }

void ASLRangedBasicProjectile::SetProjectileSize(float Val) { StaticMeshComponent->SetRelativeScale3D(FVector(ProjectileHeight, Val, ProjectileLength)); }

// Called when the game starts or when spawned
void ASLRangedBasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation((GetActorLocation() + SceneComponent->GetForwardVector() * BasicAttackDisjoints[0] + SceneComponent->GetRightVector() * BasicAttackDisjoints[1]));
	StartingLocation = GetActorLocation();
}

void ASLRangedBasicProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->Implements<USLDamageable>()) Destroy();
	else if (Cast<ISLDamageable>(OtherActor) != Cast<ISLDamageable>(Origin))
	{
		if (Cast<ASLGod>(Origin))
		{
			ASLGod* God = Cast<ASLGod>(Origin);

			if (God->GetIsPhysicalDamage()) Cast<ISLDamageable>(OtherActor)->SetCurrentHealth((God->GetCurrentBasicAttackDamage() + God->GetPhysicalPower() * God->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier, Origin);
			else Cast<ISLDamageable>(OtherActor)->SetCurrentHealth((God->GetCurrentBasicAttackDamage() + God->GetMagicalPower() * God->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier, Origin);
		}
		Destroy();
	}
}

// Called every frame
void ASLRangedBasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + SceneComponent->GetForwardVector() * ProjectileSpeed * 100 * DeltaTime);
	if (FVector::Dist(StartingLocation, GetActorLocation()) >= (ProjectileRange - ProjectileLength/2) * 100) Destroy();
}