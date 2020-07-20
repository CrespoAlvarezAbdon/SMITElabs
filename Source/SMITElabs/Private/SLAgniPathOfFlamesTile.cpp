// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAgniPathOfFlamesTile.h"

// Sets default values
ASLAgniPathOfFlamesTile::ASLAgniPathOfFlamesTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASLAgniPathOfFlamesTile::OnOverlapBegin);
}

void ASLAgniPathOfFlamesTile::SetTileMaterial(UMaterial* Val) { StaticMeshComponent->SetMaterial(0, Val); }

void ASLAgniPathOfFlamesTile::SetOrigin(AActor* Val) { Origin = Val; }

AActor* ASLAgniPathOfFlamesTile::GetOrigin() { return Origin; }

// Called when the game starts or when spawned
void ASLAgniPathOfFlamesTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASLAgniPathOfFlamesTile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (Cast<ASLGod>(OtherActor) && !Cast<ASLAgni>(Origin)->GetPathOfFlamesTargets().Contains(Cast<ASLGod>(OtherActor)) && OtherActor != Origin)
	{
		Cast<ASLAgni>(Origin)->AddPathOfFlamesTarget(Cast<ASLGod>(OtherActor));
	}
}

// Called every frame
void ASLAgniPathOfFlamesTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

