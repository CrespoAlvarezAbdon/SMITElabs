// Fill out your copyright notice in the Description page of Project Settings.


#include "SLTowerProjectile.h"
#include "SMITElabs/Public/SLVulnerable.h"
#include "SMITElabs/Public/SLTower.h"

// Sets default values
ASLTowerProjectile::ASLTowerProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	ProjectileMovementComponent->InitialSpeed = 7500;
	ProjectileMovementComponent->MaxSpeed = 7500;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingAccelerationMagnitude = 1000000;

	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLTowerProjectile::OnOverlapBegin);
}

void ASLTowerProjectile::SetCurrentTarget(ISLVulnerable* Val) {	CurrentTarget = Val; ProjectileMovementComponent->HomingTargetComponent = Cast<AActor>(CurrentTarget)->GetRootComponent(); }

void ASLTowerProjectile::SetOrigin(ASLTower* Val) { Origin = Val; }

void ASLTowerProjectile::SetTowerDamageMultiplier(float Val) { TowerDamageMultiplier = Val; }

void ASLTowerProjectile::SetProjectileSpeed(float Val) { ProjectileMovementComponent->InitialSpeed = Val; ProjectileMovementComponent->MaxSpeed = Val; }

// Called when the game starts or when spawned
void ASLTowerProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASLTowerProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherComp == Cast<AActor>(CurrentTarget)->FindComponentByClass<UCapsuleComponent>())
	{
		float TotalProtections = (CurrentTarget->GetPhysicalProtections()) * (1 - Origin->GetPercentagePhysicalPenetration()) - Origin->GetFlatPhysicalPenetration() > 0 ? (CurrentTarget->GetPhysicalProtections()) * (1 - Origin->GetPercentagePhysicalPenetration()) - Origin->GetFlatPhysicalPenetration() : 0;
		CurrentTarget->TakeHealthDamage(((Origin->GetCurrentBasicAttackDamage() + Origin->GetPhysicalPower() * Origin->GetBasicAttackPowerScaling()) * TowerDamageMultiplier) * (100 / (TotalProtections + 100)), Origin);
		Destroy();
	}
}

// Called every frame
void ASLTowerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

