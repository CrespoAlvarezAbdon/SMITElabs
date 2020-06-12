// Fill out your copyright notice in the Description page of Project Settings.


#include "SLTower.h"
#include "SMITElabs/Public/SLTowerProjectile.h"

// Sets default values
ASLTower::ASLTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	TowerBaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerBaseComponent"));
	TowerBaseComponent->SetupAttachment(RootComponent);

	TowerShaftComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerShaftComponent"));
	TowerShaftComponent->SetupAttachment(RootComponent);

	TowerHeadComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerHeadComponent"));
	TowerHeadComponent->SetupAttachment(RootComponent);

	TowerShotRangeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerShotRangeComponent"));
	TowerShotRangeComponent->SetupAttachment(RootComponent);

	TowerShotRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLTower::OnOverlapBegin);
	TowerShotRangeComponent->OnComponentEndOverlap.AddDynamic(this, &ASLTower::OnOverlapEnd);

	FireTowerShotTimerDelegate.BindUFunction(this, FName("FireTowerShot"), false);
	BeginPlayTimerDelegate.BindUFunction(this, FName("InitialSearchForTarget"), false);

	CurrentHealth = 2000;
	CurrentBasicAttackDamage = 215;
}

void ASLTower::TakeHealthDamage(float Val, ISLDangerous* Origin)
{
	float OriginalHealth = CurrentHealth;
	if (Origin->GetIsPhysicalDamage()) Val *= 0.85; else Val *= 1.2;
	ISLVulnerable::TakeHealthDamage(Val, Origin);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s dealt %f damage to %s (%f -> %f)"), *Cast<AActor>(Origin)->GetName(), Val, *this->GetName(), OriginalHealth, CurrentHealth));
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Tower [%s] has been destroyed by %s!"), *this->GetName(), *Cast<AActor>(Origin)->GetName()));
		Destroy();
	}
}

// Called when the game starts or when spawned
void ASLTower::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireTowerShotTimerHandle, BeginPlayTimerDelegate, .1, false);
}

void ASLTower::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	SearchForTarget(true);
}

void ASLTower::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PreviousTarget = CurrentTarget;
	CurrentTarget = nullptr;
	SearchForTarget(false);
}

void ASLTower::SearchForTarget(bool bSetTimer)
{
	if (CurrentTarget == nullptr)
	{
		TArray<AActor*> OverlappingActors;
		TowerShotRangeComponent->GetOverlappingActors(OverlappingActors);
		float ShortestDistance{ -1 };
		for (AActor* var : OverlappingActors)
		{
			if (Cast<ISLVulnerable>(var) && var != this)
			{
				if (FVector::Dist2D(this->GetActorLocation(), var->GetActorLocation()) < ShortestDistance || ShortestDistance == -1)
				{
					CurrentTarget = Cast<ISLVulnerable>(var);
					ShortestDistance = FVector::Dist(this->GetActorLocation(), var->GetActorLocation());
				}
			}
		}
		if (CurrentTarget != nullptr && bSetTimer) GetWorld()->GetTimerManager().SetTimer(FireTowerShotTimerHandle, FireTowerShotTimerDelegate, 1, false);
		if (PreviousTarget != nullptr && PreviousTarget != CurrentTarget) TowerDamageMultiplier = 1;
		else PreviousTarget = nullptr;
	}
}

void ASLTower::InitialSearchForTarget()
{
	SearchForTarget(true);
}

void ASLTower::FireTowerShot()
{
	if (CurrentTarget != nullptr)
	{
		ASLTowerProjectile* SpawnedTowerProjectile = GetWorld()->SpawnActorDeferred<ASLTowerProjectile>(TowerProjectile, TowerHeadComponent->GetComponentTransform());
		SpawnedTowerProjectile->SetCurrentTarget(CurrentTarget);
		SpawnedTowerProjectile->SetOrigin(this);
		SpawnedTowerProjectile->SetTowerDamageMultiplier(TowerDamageMultiplier);
		UGameplayStatics::FinishSpawningActor(SpawnedTowerProjectile, SpawnedTowerProjectile->GetTransform());
		TowerDamageMultiplier += TowerDamageMultiplierIncrement;
		GetWorld()->GetTimerManager().SetTimer(FireTowerShotTimerHandle, FireTowerShotTimerDelegate, 1, false);
	}
}

// Called every frame
void ASLTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}