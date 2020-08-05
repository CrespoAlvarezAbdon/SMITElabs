// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAgni.h"
#include "SMITElabs/Public/SLAgniNoxiousFumes.h"
#include "SMITElabs/Public/SLAgniFlameWave.h"
#include "SMITElabs/Public/SLAgniPathOfFlamesTile.h"
#include "SMITElabs/Public/SLAgniRainFire.h"

ASLAgni::ASLAgni()
{
	HaloOriginComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HaloOriginComponent"));
	HaloOriginComponent->SetupAttachment(RootComponent);

	for (int i = 0; i < 3; i++)
	{
		HaloMeteorComponents.Add(CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Halo Meteor Component %i"), i + 1)));
		HaloMeteorComponents[i]->SetupAttachment(HaloOriginComponent);
		HaloMeteorComponents[i]->SetVisibility(false);
	}

	UnitName = "Agni";
	BaseMovementSpeed = 355;
	BaseBasicAttackDamage = 34;
	BasicAttackDamagePerLevel = 1.5;
	BasicAttackPowerScaling = 0.2;
	BaseBasicAttackSpeed = 1;
	BasicAttackSpeedPerLevel = .012;
	BasicAttackRefireProgression = { 1 };
	BasicAttackPrefireProgression = { .15 };
	BasicAttackRangeProgression = { 55 };
	BasicAttackDamageProgression = { 1 };
	BasicAttackDisjointProgression = { 0, 0 };
	RangedBasicAttackProjectileSizeProgression = { 3 };
	RangedBasicAttackProjectileSpeedProgression = { 100 };
	bCleaveProgression = { false };
	CleaveDamageProgression = { 0 };
	RangedCleaveRangeProgression = { 0 };
	bIsBasicAttackRangedProgression = { true };
	bHasScalingPrefireProgression = { false };
	bIsPhysicalDamage = false;
	BaseHealth = 360;
	BasePhysicalProtections = 11;
	PhysicalProtectionsPerLevel = 2.6;
	BaseMagicalProtections = 30;
	MagicalProtectionsPerLevel = .9;
	BaseHealthPerFive = 7;
	HealthPerFivePerLevel = .47;

	//Essential for proper implementation of editing ability stuff in blueprints and at runtime
	SetAbilityArrays();
	SetAbilityTargeterArrays();

	FlameWavePrefireTimerDelegate.BindUFunction(this, FName("UseFlameWave"), false);

	SetActorTickEnabled(true);
}

TArray<ASLGod*> ASLAgni::GetCombustionTargets()
{
	return CombustionTargets;
}

TArray<ASLGod*> ASLAgni::GetPathOfFlamesTargets()
{
	return PathOfFlamesTargets;
}

void ASLAgni::AddCombustionTarget(ASLGod* CombustionTarget)
{
	CombustionTargets.Push(CombustionTarget);
	CombustionTimerHandles.Push(FTimerHandle());
	CombustionTicks.Push(6);
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("DealCombustionDamage"), CombustionTargets.Last(), CombustionTimerHandles.Last(), CombustionTicks.Last());
	GetWorldTimerManager().SetTimer(CombustionTimerHandles.Last(), TimerDelegate, .5, false);
}

void ASLAgni::AddPathOfFlamesTarget(ASLGod* PathOfFlamesTarget)
{
	PathOfFlamesTargets.Push(PathOfFlamesTarget);
	PathOfFlamesTimerHandles.Push(FTimerHandle());
	PathOfFlamesTicks.Push(4);
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("DealPathOfFlamesDamage"), PathOfFlamesTargets.Last(), PathOfFlamesTimerHandles.Last(), PathOfFlamesTicks.Last());
	GetWorldTimerManager().SetTimer(PathOfFlamesTimerHandles.Last(), TimerDelegate, .5, false);
}

void ASLAgni::SetGodLevel(int Val)
{
	Super::SetGodLevel(Val);

	if (GetWorldTimerManager().IsTimerActive(AbilityCooldownTimerHandles[3])) GetWorldTimerManager().PauseTimer(AbilityCooldownTimerHandles[3]);
}

void ASLAgni::UseNoxiousFumes()
{
	if (NoxiousFumes != nullptr) NoxiousFumes->Destroy();
	NoxiousFumes = GetWorld()->SpawnActorDeferred<ASLAgniNoxiousFumes>(NoxiousFumesTile, NoxiousFumesSpawnTransform);
	NoxiousFumes->SetOrigin(this);
	NoxiousFumes->SetDamage(NoxiousFumesDamage[AbilitySlotPoints[0] - 1]);
	NoxiousFumes->SetScaling(NoxiousFumesScaling);
	NoxiousFumes->SetLifeSpan(10);
	UGameplayStatics::FinishSpawningActor(NoxiousFumes, NoxiousFumesSpawnTransform);
	bCanFireAbility = true;
	bCanBasicAttack = true;
	BeginFireBasicAttack();
	StartAbilityCooldown(0);
	if (AbilityKeyDown > -1) AimAbility(AbilityKeyDown);
}

void ASLAgni::UseFlameWave()
{
	ASLAgniFlameWave* FlameWave = GetWorld()->SpawnActorDeferred<ASLAgniFlameWave>(FlameWaveProjectile, GetTransform());
	FlameWave->SetOrigin(this);
	FlameWave->SetDamage(FlameWaveDamage[AbilitySlotPoints[1] - 1]);
	FlameWave->SetScaling(FlameWaveScaling);
	if (CombustionCount >= 4)
	{ 
		ConsumeCombustionStacks();
		FlameWave->SetBHasCombustion(true);
	}
	else FlameWave->SetBHasCombustion(false);
	UGameplayStatics::FinishSpawningActor(FlameWave, FlameWave->GetTransform());
	bCanFireAbility = true;
	bCanBasicAttack = true;
	BeginFireBasicAttack();
	StartAbilityCooldown(1);
	if (AbilityKeyDown > -1) AimAbility(AbilityKeyDown);
}

void ASLAgni::UsePathOfFlames()
{
	bCanMove = false;
	bCanFireAbility = false;
	bCanBasicAttack = false;
	bUseControllerRotationYaw = false;
	CharacterMovementComponent->BrakingFrictionFactor = 0;
	bIsUsingPathOfFlames = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ASLAgni::EndPathOfFlames()
{
	PathOfFlamesSpawnDistance = 0;
	PathOfFlamesDistanceTravelled = 0;
	bCanMove = true;
	CharacterMovementComponent->bUseControllerDesiredRotation = true;
	CharacterMovementComponent->BrakingFrictionFactor = 2;
	bIsUsingPathOfFlames = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	StartAbilityCooldown(2);
}

void ASLAgni::UseRainFire()
{
	bCanFireAbility = false;
	bCanBasicAttack = false;
	bCanMove = false;
	GetWorldTimerManager().SetTimer(EndRainFireTimerHandle, this, &ASLAgni::EndRainFire, 0.5, true);
	ASLAgniRainFire* RainFire = GetWorld()->SpawnActorDeferred<ASLAgniRainFire>(RainFireTile, GetActorTransform());
	RainFire->SetOrigin(this);
	RainFire->SetDamage(RainFireDamage[AbilitySlotPoints[3] - 1]);
	RainFire->SetScaling(RainFireScaling);
	if (CombustionCount >= 4)
	{
		ConsumeCombustionStacks();
		RainFire->SetBHasCombustion(true);
	}
	else RainFire->SetBHasCombustion(false);
	UGameplayStatics::FinishSpawningActor(RainFire, RainFire->GetActorTransform());
	StartAbilityCooldown(3);
	HaloMeteorComponents[CurrentAbilityCharges[3]]->SetVisibility(false);
	
}

void ASLAgni::EndRainFire()
{
	bCanFireAbility = true;
	bCanBasicAttack = true;
	bCanMove = true;
}

void ASLAgni::OnBasicAttackHit(TArray<ISLVulnerable*> Targets)
{
	Super::OnBasicAttackHit(Targets);

	for (ISLVulnerable* CurrentTarget : Targets)
	{
		if (CombustionCount < 4)
		{ 
			++CombustionCount;
			if (CombustionCount >= 4){ GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Combustion is ready!")); StaticMeshComponent->SetMaterial(0, MAgniCombustionStacked); }
			else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Combustion Passive Count: %i"), CombustionCount));
		}
	}
}

void ASLAgni::FireAbility(int AbilitySlot)
{
	AbilityKeyDown = -1;
	if (AbilitySlotAbilities[AbilitySlot] == PrimedAbility && (int)CurrentMana >= (int)CurrentAbilityManaCosts[AbilitySlot] && bCanFireAbility)
	{
		CancelAbility();
		switch (AbilitySlot)
		{
		case 0:      
			NoxiousFumesSpawnTransform = TargeterLocationComponent->GetComponentTransform();
			GetWorldTimerManager().SetTimer(NoxiousFumesPrefireTimerHandle, this, &ASLAgni::UseNoxiousFumes, .15, false);
			bCanFireAbility = false;
			bCanBasicAttack = false;
			break;
		case 1:
			GetWorldTimerManager().SetTimer(FlameWavePrefireTimerHandle, FlameWavePrefireTimerDelegate, .5, false);
			bCanFireAbility = false;
			bCanBasicAttack = false; 
			break;
		case 2:
			UsePathOfFlames();
			break;
		case 3:
			UseRainFire();
			break;
		}
	}
}

void ASLAgni::ConsumeCombustionStacks()
{
	CombustionCount = 0;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Combustion Stacks Consumed."));
	StaticMeshComponent->SetMaterial(0, MAgni);
}

void ASLAgni::DealCombustionDamage(ASLGod* CombustionTarget, FTimerHandle CombustionTimerHandle, int& CombustionTick)
{
	CombustionTarget->TakeHealthDamage((CombustionDamage + GetMagicalPower() * CombustionScaling) * (100 / (CalculateTotalProtections(CombustionTarget) + 100)), this);
	FTimerDelegate TimerDelegate;
	--CombustionTick;
	if(CombustionTick > 0)
	{
		TimerDelegate.BindUFunction(this, FName("DealCombustionDamage"), CombustionTarget, CombustionTimerHandle, CombustionTick);
		GetWorldTimerManager().SetTimer(CombustionTimerHandle, TimerDelegate, .5, false);
	}
	else
	{
		CombustionTargets.RemoveAt(0);
		CombustionTimerHandles.RemoveAt(0);
		CombustionTicks.RemoveAt(0);
	}
}

void ASLAgni::DealPathOfFlamesDamage(ASLGod* PathOfFlamesTarget, FTimerHandle PathOfFlamesTimerHandle, int& PathOfFlamesTick)
{
	//The way this ability deals damage to multiple targets that rewalk over the fire is very buggy and should really be redone. I'm just going to leave it as it is for now.

	PathOfFlamesTarget->TakeHealthDamage((PathOfFlamesDamage[AbilitySlotPoints[2] - 1] + GetMagicalPower() * CombustionScaling) * (100 / (CalculateTotalProtections(PathOfFlamesTarget) + 100)), this);
	FTimerDelegate TimerDelegate;
	--PathOfFlamesTick;
	TArray<AActor*> OverlappingActors;
	PathOfFlamesTarget->GetOverlappingActors(OverlappingActors);
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (Cast<ASLAgniPathOfFlamesTile>(OverlappingActor) && Cast<ASLAgniPathOfFlamesTile>(OverlappingActor)->GetOrigin() == this)
		{
			PathOfFlamesTick = 4;
			PathOfFlamesTargets.Swap(0, PathOfFlamesTargets.Num() - 1);
			PathOfFlamesTimerHandles.Swap(0, PathOfFlamesTimerHandles.Num() - 1);
			PathOfFlamesTicks.Swap(0, PathOfFlamesTicks.Num() - 1);
		}
	}
	if (PathOfFlamesTick > 0)
	{
		TimerDelegate.BindUFunction(this, FName("DealPathOfFlamesDamage"), PathOfFlamesTarget, PathOfFlamesTimerHandle, PathOfFlamesTick);
		GetWorldTimerManager().SetTimer(PathOfFlamesTimerHandle, TimerDelegate, .5, false);
	}
	else
	{
		PathOfFlamesTargets.RemoveAt(0);
		PathOfFlamesTimerHandles.RemoveAt(0);
		PathOfFlamesTicks.RemoveAt(0);
	}
}

void ASLAgni::OnAbilityCooldownEnded(int AbilityID)
{
	Super::OnAbilityCooldownEnded(AbilityID);

	if (AbilityID == 3) HaloMeteorComponents[CurrentAbilityCharges[AbilityID] - 1]->SetVisibility(true);
}

void ASLAgni::LevelAbility(int AbilitySlot)
{
	if (AbilitySlot == 3 && AbilitySlotPoints[3] == 0)
	{
		if (GetWorldTimerManager().IsTimerPaused(AbilityCooldownTimerHandles[3])) GetWorldTimerManager().UnPauseTimer(AbilityCooldownTimerHandles[3]);
		else if (bSetAbilityChargesAtLevelOne[3]) HaloMeteorComponents[0]->SetVisibility(true);
	}

	Super::LevelAbility(AbilitySlot);
}

void ASLAgni::Tick(float DeltaTime)
{
	HaloOriginComponent->SetRelativeRotation(HaloOriginComponent->GetRelativeRotation() + FRotator(0, 90 * DeltaTime, 0));
	if (bIsUsingPathOfFlames)
	{
		if (PathOfFlamesSpawnDistance != 0)
		{
			PathOfFlamesDistanceTravelled += 10000 * DeltaTime;
			while (PathOfFlamesDistanceTravelled >= PathOfFlamesSpawnDistance)
			{
				FTransform SpawnTransform = GetActorTransform();
				SpawnTransform.SetLocation(GetActorLocation() - GetActorForwardVector() * (PathOfFlamesDistanceTravelled - PathOfFlamesSpawnDistance));
				ASLAgniPathOfFlamesTile* FlameTile = GetWorld()->SpawnActorDeferred<ASLAgniPathOfFlamesTile>(PathOfFlamesTile, SpawnTransform);
				FlameTile->SetOrigin(this);
				UGameplayStatics::FinishSpawningActor(FlameTile, FlameTile->GetTransform());
				if ((int)PathOfFlamesSpawnDistance % 2000 == 1000) FlameTile->SetTileMaterial(MPathOfFlamesLight);
				else FlameTile->SetTileMaterial(MPathOfFlamesDark);
				FlameTile->SetLifeSpan(3);
				if (PathOfFlamesSpawnDistance >= 5000)
				{
					SetActorLocation(GetActorLocation() - GetActorForwardVector() * (PathOfFlamesDistanceTravelled - PathOfFlamesSpawnDistance));
					EndPathOfFlames();
					return;
				}
				PathOfFlamesSpawnDistance += 1000;
			}
		}
		else
		{
			ASLAgniPathOfFlamesTile* FlameTile = GetWorld()->SpawnActorDeferred<ASLAgniPathOfFlamesTile>(PathOfFlamesTile, GetActorTransform());
			FlameTile->SetOrigin(this);
			UGameplayStatics::FinishSpawningActor(FlameTile, FlameTile->GetTransform());
			if ((int)PathOfFlamesSpawnDistance % 2000 == 1000) FlameTile->SetTileMaterial(MPathOfFlamesLight);
			else FlameTile->SetTileMaterial(MPathOfFlamesDark);
			FlameTile->SetLifeSpan(3);
			PathOfFlamesSpawnDistance += 1000;
		}
		CharacterMovementComponent->Velocity = GetActorForwardVector() * 10000;
	}
	else if (!bUseControllerRotationYaw && FMath::IsNearlyEqual(GetControlRotation().GetDenormalized().Yaw, StaticMeshComponent->GetComponentRotation().GetDenormalized().Yaw, .1f))
	{
		bCanFireAbility = true;
		bCanBasicAttack = true;
		bUseControllerRotationYaw = true;
		CharacterMovementComponent->bUseControllerDesiredRotation = false;
		if (AbilityKeyDown > -1) AimAbility(AbilityKeyDown);
	}
}
