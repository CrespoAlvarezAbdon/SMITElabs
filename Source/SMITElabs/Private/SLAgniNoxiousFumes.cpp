// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAgniNoxiousFumes.h"

// Sets default values
ASLAgniNoxiousFumes::ASLAgniNoxiousFumes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLAgniNoxiousFumes::OnOverlapBegin);
}

void ASLAgniNoxiousFumes::SetOrigin(ASLGod* Val) { Origin = Val; }

void ASLAgniNoxiousFumes::SetDamage(float Val) { Damage = Val; }

void ASLAgniNoxiousFumes::SetScaling(float Val) { Scaling = Val; }

// Called when the game starts or when spawned
void ASLAgniNoxiousFumes::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(DealFumesDamageTimerHandle, this, &ASLAgniNoxiousFumes::DealFumesDamage, 1, true, 0);
}

void ASLAgniNoxiousFumes::DealFumesDamage()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (Cast<ASLGod>(OverlappingActor) && OverlappingActor != Origin) Cast<ASLGod>(OverlappingActor)->TakeHealthDamage((Damage + Origin->GetMagicalPower() * Scaling) * (100 / (Origin->CalculateTotalProtections(Cast<ASLGod>(OverlappingActor)) + 100)), Origin);
	}
	if (StaticMeshComponent->GetCollisionResponseToChannel(ECC_WorldStatic) == ECR_Ignore) StaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
}

void ASLAgniNoxiousFumes::DestroyFumes()
{
	Destroy();
}

void ASLAgniNoxiousFumes::ActivateStun()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (Cast<ASLGod>(OverlappingActor) && OverlappingActor != Origin) Cast<ASLGod>(OverlappingActor)->BecomeStunned(1);
	}
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetMaterial(0, MNoxiousFumesActive);
	GetWorldTimerManager().SetTimer(DestroyFumesTimerHandle, this, &ASLAgniNoxiousFumes::DestroyFumes, .5, false);
}

void ASLAgniNoxiousFumes::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (Cast<ASLAgniFlameWave>(OtherActor) || Cast<ASLAgniPathOfFlamesTile>(OtherActor) || Cast<ASLAgniRainFire>(OtherActor))
		ActivateStun();
}
