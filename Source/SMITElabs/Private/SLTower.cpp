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
	TowerHeadComponent->SetRelativeLocation(FVector(0, 0, 2250));

	TowerShotRangeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerShotRangeComponent"));
	TowerShotRangeComponent->SetupAttachment(RootComponent);

	TowerShotRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLTower::OnOverlapBegin);
	TowerShotRangeComponent->OnComponentEndOverlap.AddDynamic(this, &ASLTower::OnOverlapEnd);

	PrepareTowerShotTimerDelegate.BindUFunction(this, FName("PrepareTowerShot"), false);
	FireTowerShotTimerDelegate.BindUFunction(this, FName("FireTowerShot"), false);
	BeginPlayTimerDelegate.BindUFunction(this, FName("InitialSearchForTarget"), false);
	TowerHealthRegenTimerDelegate.BindUFunction(this, FName("HealTower"), false);

	BaseHealth = 2000;
	CurrentHealth = BaseHealth;
	TowerMaxRegenHealth = BaseHealth * .7;
	CurrentBasicAttackDamage = 215;
	PhysicalProtections = 125;
	MagicalProtections = 125;
	if (!bHasShaft) { TowerShaftComponent->ToggleVisibility(false); TowerHeadComponent->SetRelativeLocation(FVector(TowerHeadComponent->GetRelativeLocation().X, TowerHeadComponent->GetRelativeLocation().Y, TowerHeadComponent->GetRelativeLocation().Z - 1250)); }
	bIsOrder = bIsOrderBP;
}

void ASLTower::TakeHealthDamage(float Val, ISLDangerous* Origin)
{
	float OriginalHealth = CurrentHealth;
	if (Origin->GetIsPhysicalDamage()) Val *= 0.85; else Val *= 1.2;
	Val *= .5;
	ISLVulnerable::TakeHealthDamage(Val, Origin);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s dealt %f damage to %s (%f -> %f) [Backdoor Protections!]"), *Cast<AActor>(Origin)->GetName(), Val, *this->GetName(), OriginalHealth, CurrentHealth));
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Structure %s has been destroyed by %s!"), *this->GetName(), *Cast<AActor>(Origin)->GetName()));
		Destroy();
	}
}

// Called when the game starts or when spawned
void ASLTower::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireTowerShotTimerHandle, BeginPlayTimerDelegate, .1, false);
	if (bHasTowerHPS) GetWorld()->GetTimerManager().SetTimer(TowerHealthRegenTimerHandle, TowerHealthRegenTimerDelegate, 1, false);
}

void ASLTower::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	SearchForTarget(true);
}

void ASLTower::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ISLVulnerable>(OtherActor) && OtherActor == Cast<AActor>(CurrentTarget)) 
	{
		TowerDamageMultiplier = 1;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("%s is no longer being targeted by %s."), *OtherActor->GetName(), *this->GetName()));
		PreviousTarget = CurrentTarget;
		CurrentTarget = nullptr;
		SearchForTarget(false);
	}
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
			if (Cast<ISLVulnerable>(var) && var != this && Cast<ISLIdentifiable>(var)->GetBIsOrder() != bIsOrder)
			{
				if (FVector::Dist2D(this->GetActorLocation(), var->GetActorLocation()) < ShortestDistance || ShortestDistance == -1)
				{
					CurrentTarget = Cast<ISLVulnerable>(var);
					ShortestDistance = FVector::Dist(this->GetActorLocation(), var->GetActorLocation());
				}
			}
		}
		if (CurrentTarget != nullptr) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("%s is being targeted by %s!"), *Cast<AActor>(CurrentTarget)->GetName(), *this->GetName()));
		if (CurrentTarget != nullptr && bSetTimer && !GetWorld()->GetTimerManager().IsTimerActive(FireTowerShotTimerHandle))
		{ 
			PrepareTowerShot();
		}
		else PreviousTarget = nullptr;
	}
}

void ASLTower::InitialSearchForTarget()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTowerShotTimerHandle);
	SearchForTarget(true);
}

void ASLTower::PrepareTowerShot()
{
	if (CurrentTarget != nullptr)
	{
		SpawnedTowerProjectile = GetWorld()->SpawnActorDeferred<ASLTowerProjectile>(TowerProjectile, TowerHeadComponent->GetComponentTransform());
		SpawnedTowerProjectile->SetCurrentTarget(CurrentTarget);
		SpawnedTowerProjectile->SetOrigin(this);
		SpawnedTowerProjectile->SetTowerDamageMultiplier(TowerDamageMultiplier);
		GetWorld()->GetTimerManager().SetTimer(FireTowerShotTimerHandle, FireTowerShotTimerDelegate, .25, false);
	}
}

void ASLTower::FireTowerShot()
{
	UGameplayStatics::FinishSpawningActor(SpawnedTowerProjectile, SpawnedTowerProjectile->GetTransform());
	TowerDamageMultiplier += TowerDamageMultiplierIncrement;
	GetWorld()->GetTimerManager().SetTimer(FireTowerShotTimerHandle, PrepareTowerShotTimerDelegate, 1, false);
}

void ASLTower::HealTower()
{
	GetWorld()->GetTimerManager().SetTimer(TowerHealthRegenTimerHandle, TowerHealthRegenTimerDelegate, 1, false);
	float OriginalHealth = CurrentHealth;
	if (CurrentHealth >= TowerMaxRegenHealth) return;
	CurrentHealth += TowerHealthPerSecond;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Structure %s passively healed for %f Health (%f -> %f)"), *this->GetName(), CurrentHealth - OriginalHealth, OriginalHealth, CurrentHealth));
}

// Called every frame
void ASLTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}