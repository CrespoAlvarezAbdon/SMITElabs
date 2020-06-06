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

void ASLRangedBasicProjectile::SetOrigin(ASLGod* Val) { Origin = Val; }

void ASLRangedBasicProjectile::SetDamageProgressionMultiplier(float Val) { DamageProgressionMultiplier = Val; }

void ASLRangedBasicProjectile::SetProjectileSpeed(float Val) { ProjectileSpeed = Val; }

void ASLRangedBasicProjectile::SetProjectileRange(float Val) { ProjectileRange = Val; }

void ASLRangedBasicProjectile::SetProjectileSize(float Val) { ProjectileSize = Val;  StaticMeshComponent->SetRelativeScale3D(FVector(ProjectileHeight, ProjectileSize, ProjectileLength)); 	WallCollisionComponent->SetBoxExtent(FVector(ProjectileLength * 50, 25, 25)); }

void ASLRangedBasicProjectile::SetCleave(bool Val) { bCleave = Val; }

void ASLRangedBasicProjectile::SetCleaveDamage(float Val) { CleaveDamage = Val; }

void ASLRangedBasicProjectile::SetCleaveRange(float Val) { CleaveRange = Val; CleaveCollisionComponent->SetSphereRadius(CleaveRange * 100); }

void ASLRangedBasicProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// TODO Have Projectiles use a different collision for walls, since they don't seem to use the same hitbox used for damageable targets in-game, 
	//      more like a ~<1 unit wide shot similar to how Melee only hits a target if both God central points have LoS.

	if (!OtherActor->Implements<USLDamageable>() || Cast<ASLRangedBasicProjectile>(OtherActor)) return;
	else if (Cast<ISLDamageable>(OtherActor) != Cast<ISLDamageable>(Origin))
	{
		if (Cast<ASLGod>(Origin))
		{
			ASLGod* God = Cast<ASLGod>(Origin);
			ISLDamageable* DamagedTarget = Cast<ISLDamageable>(OtherActor);
			if (God->GetIsPhysicalDamage())
			{
				float TotalProtections = (DamagedTarget->GetPhysicalProtections()) * (1  - God->GetPercentagePhysicalPenetration()) - God->GetFlatPhysicalPenetration() > 0 ? (DamagedTarget->GetPhysicalProtections()) * (1 - God->GetPercentagePhysicalPenetration()) - God->GetFlatPhysicalPenetration() : 0;
				DamagedTarget->SetCurrentHealth(((God->GetCurrentBasicAttackDamage() + God->GetPhysicalPower() * God->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier) * (100 / (TotalProtections + 100)), Origin);
			}
			else
			{
				float TotalProtections = (DamagedTarget->GetMagicalProtections()) * (1 - God->GetPercentageMagicalPenetration()) - God->GetFlatMagicalPenetration() > 0 ? (DamagedTarget->GetMagicalProtections()) * (1 - God->GetPercentageMagicalPenetration()) - God->GetFlatMagicalPenetration() : 0;
				DamagedTarget->SetCurrentHealth(((God->GetCurrentBasicAttackDamage() + God->GetMagicalPower() * God->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier) * (100 / (TotalProtections + 100)), Origin);
			}
			if (bCleave)
			{
				CleaveCollisionComponent->SetWorldLocation(OtherActor->GetActorLocation());
				TArray<AActor*> OverlappingActors;
				CleaveCollisionComponent->GetOverlappingActors(OverlappingActors);
				for (AActor* var : OverlappingActors)
				{
					if (Cast<ISLDamageable>(var) && var != OtherActor && var != Origin)
					{
						FHitResult HitResult;
						if (!GetWorld()->LineTraceSingleByChannel(HitResult, OtherActor->GetActorLocation(), var->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1))
						{
							DamagedTarget = Cast<ISLDamageable>(var);
							if (God->GetIsPhysicalDamage())
							{
								float TotalProtections = (DamagedTarget->GetPhysicalProtections()) * (1 - God->GetPercentagePhysicalPenetration()) - God->GetFlatPhysicalPenetration() > 0 ? (DamagedTarget->GetPhysicalProtections()) * (1 - God->GetPercentagePhysicalPenetration()) - God->GetFlatPhysicalPenetration() : 0;
								DamagedTarget->SetCurrentHealth(((God->GetCurrentBasicAttackDamage() + God->GetPhysicalPower() * God->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier * CleaveDamage) * (100 / (TotalProtections + 100)), Origin);
							}
							else
							{
								float TotalProtections = (DamagedTarget->GetMagicalProtections()) * (1 - God->GetPercentageMagicalPenetration()) - God->GetFlatMagicalPenetration() > 0 ? (DamagedTarget->GetMagicalProtections()) * (1 - God->GetPercentageMagicalPenetration()) - God->GetFlatMagicalPenetration() : 0;
								DamagedTarget->SetCurrentHealth(((God->GetCurrentBasicAttackDamage() + God->GetMagicalPower() * God->GetBasicAttackPowerScaling()) * DamageProgressionMultiplier * CleaveDamage) * (100 / (TotalProtections + 100)), Origin);
							}
						}
					}
				}
			}
		}
		Destroy();
	}
}

void ASLRangedBasicProjectile::OnWallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->Implements<USLDamageable>() && OtherActor != this) { Destroy(); return; }
}

// Called every frame
void ASLRangedBasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + SceneComponent->GetForwardVector() * ProjectileSpeed * 100 * DeltaTime);
	if (FVector::Dist(StartingLocation, GetActorLocation()) >= (ProjectileRange - ProjectileLength/2) * 100) Destroy();
}