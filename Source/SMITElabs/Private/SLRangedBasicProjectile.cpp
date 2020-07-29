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
	CleaveCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CleaveCollisionComponent"));
	CleaveCollisionComponent->SetupAttachment(RootComponent);
	WallCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("WallCollisionComponent"));
	WallCollisionComponent->SetupAttachment(RootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLRangedBasicProjectile::OnOverlapBegin);
	WallCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLRangedBasicProjectile::OnWallHit);
}

float ASLRangedBasicProjectile::GetProjectileLength() { return ProjectileLength; }

// Called when the game starts or when spawned
void ASLRangedBasicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation((GetActorLocation() + SceneComponent->GetForwardVector() * BasicAttackDisjoints[0] + SceneComponent->GetRightVector() * BasicAttackDisjoints[1]));
	StartingLocation = GetActorLocation();
}

void ASLRangedBasicProjectile::SetBasicAttackDisjoints(TArray<float> Val)
{
	BasicAttackDisjoints[0] = Val[0];
	BasicAttackDisjoints[1] = Val[1];	
}

void ASLRangedBasicProjectile::SetOrigin(ISLDangerous* Val) { Origin = Val; }

void ASLRangedBasicProjectile::SetDamageProgressionMultiplier(float Val) { DamageProgressionMultiplier = Val; }

void ASLRangedBasicProjectile::SetProjectileSpeed(float Val) { ProjectileSpeed = Val; }

void ASLRangedBasicProjectile::SetProjectileRange(float Val) { ProjectileRange = Val; }

void ASLRangedBasicProjectile::SetProjectileSize(float Val) { ProjectileSize = Val;  StaticMeshComponent->SetRelativeScale3D(FVector(ProjectileHeight, ProjectileSize, ProjectileLength)); 	WallCollisionComponent->SetBoxExtent(FVector(ProjectileLength * 50, 25, 25)); }

void ASLRangedBasicProjectile::SetCleave(bool Val) { bCleave = Val; }

void ASLRangedBasicProjectile::SetCleaveDamage(float Val) { CleaveDamage = Val; }

void ASLRangedBasicProjectile::SetCleaveRange(float Val) { CleaveRange = Val; CleaveCollisionComponent->SetSphereRadius(CleaveRange * 100); }

void ASLRangedBasicProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// TODO Clean up this ugly if/else if
	if (!OtherActor->Implements<USLVulnerable>() || Cast<ASLRangedBasicProjectile>(OtherActor)) return;
	else if (OtherActor != Cast<AActor>(Origin) && Cast<ISLIdentifiable>(Origin)->GetBIsOrder() != Cast<ISLIdentifiable>(OtherActor)->GetBIsOrder())
	{
		ISLVulnerable* DamagedTarget = Cast<ISLVulnerable>(OtherActor);
		if (Origin->GetIsPhysicalDamage())
		{
			float TotalProtections = (DamagedTarget->GetPhysicalProtections()) * (1 - Origin->GetPercentagePhysicalPenetration()) - Origin->GetFlatPhysicalPenetration() > 0 ? (DamagedTarget->GetPhysicalProtections()) * (1 - Origin->GetPercentagePhysicalPenetration()) - Origin->GetFlatPhysicalPenetration() : 0;
			DamagedTarget->TakeHealthDamage(((Origin->GetCurrentBasicAttackDamage() + Origin->GetPhysicalPower() * Origin->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier) * (100 / (TotalProtections + 100)), Origin);
		}
		else
		{
			float TotalProtections = (DamagedTarget->GetMagicalProtections()) * (1 - Origin->GetPercentageMagicalPenetration()) - Origin->GetFlatMagicalPenetration() > 0 ? (DamagedTarget->GetMagicalProtections()) * (1 - Origin->GetPercentageMagicalPenetration()) - Origin->GetFlatMagicalPenetration() : 0;
			DamagedTarget->TakeHealthDamage(((Origin->GetCurrentBasicAttackDamage() + Origin->GetMagicalPower() * Origin->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier) * (100 / (TotalProtections + 100)), Origin);
		}
		TArray<ISLVulnerable*> Targets{ DamagedTarget };
		if (bCleave)
		{
			CleaveCollisionComponent->SetWorldLocation(OtherActor->GetActorLocation());
			TArray<AActor*> OverlappingActors;
			CleaveCollisionComponent->GetOverlappingActors(OverlappingActors);
			for (AActor* var : OverlappingActors)
			{
				if (Cast<ISLVulnerable>(var) && var != OtherActor && var != Cast<AActor>(Origin))
				{
					FHitResult HitResult;
					if (!GetWorld()->LineTraceSingleByChannel(HitResult, OtherActor->GetActorLocation(), var->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1))
					{
						DamagedTarget = Cast<ISLVulnerable>(var);
						if (Origin->GetIsPhysicalDamage())
						{
							float TotalProtections = (DamagedTarget->GetPhysicalProtections()) * (1 - Origin->GetPercentagePhysicalPenetration()) - Origin->GetFlatPhysicalPenetration() > 0 ? (DamagedTarget->GetPhysicalProtections()) * (1 - Origin->GetPercentagePhysicalPenetration()) - Origin->GetFlatPhysicalPenetration() : 0;
							DamagedTarget->TakeHealthDamage(((Origin->GetCurrentBasicAttackDamage() + Origin->GetPhysicalPower() * Origin->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier * CleaveDamage) * (100 / (TotalProtections + 100)), Origin);
						}
						else
						{
							float TotalProtections = (DamagedTarget->GetMagicalProtections()) * (1 - Origin->GetPercentageMagicalPenetration()) - Origin->GetFlatMagicalPenetration() > 0 ? (DamagedTarget->GetMagicalProtections()) * (1 - Origin->GetPercentageMagicalPenetration()) - Origin->GetFlatMagicalPenetration() : 0;
							DamagedTarget->TakeHealthDamage(((Origin->GetCurrentBasicAttackDamage() + Origin->GetMagicalPower() * Origin->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier * CleaveDamage) * (100 / (TotalProtections + 100)), Origin);
						}
						Targets.Add(DamagedTarget);
					}
				}
			}
		}
		Origin->OnBasicAttackHit(Targets);
		Destroy();
	}
}

void ASLRangedBasicProjectile::OnWallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->Implements<USLVulnerable>() && OtherActor != this) Destroy();
}

// Called every frame
void ASLRangedBasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	int NumberOfMovements{ (int)(DeltaTime * 100) };
	if (NumberOfMovements == 0) ++NumberOfMovements;
	for (int i = 0; i < NumberOfMovements; i++)
	{
		if (FVector::Dist(StartingLocation, GetActorLocation() + SceneComponent->GetForwardVector() * ProjectileSpeed * 100 * (DeltaTime / NumberOfMovements)) >= (ProjectileRange - ProjectileLength) * 100)
		{
			SetActorLocation(StartingLocation + SceneComponent->GetForwardVector() * (ProjectileRange - ProjectileLength) * 100);
			Destroy();
		}
		SetActorLocation(GetActorLocation() + SceneComponent->GetForwardVector() * ProjectileSpeed * 100 * (DeltaTime / NumberOfMovements));
	}
}