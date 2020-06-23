// Fill out your copyright notice in the Description page of Project Settings.


#include "SLGod.h"
#include "SMITElabs/Public/SLRangedBasicProjectile.h"

// Sets default values
ASLGod::ASLGod()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	RangeLineComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RangeLineComponent"));
	RangeLineComponent->SetupAttachment(RootComponent);
	RangeLineComponent->SetHiddenInGame(true);

	RangedAimComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RangedAimComponent"));
	RangedAimComponent->SetupAttachment(RootComponent);
	RangedAimComponent->SetHiddenInGame(true);

	MeleeAimComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeAimComponent"));
	MeleeAimComponent->SetupAttachment(RootComponent);
	MeleeAimComponent->SetHiddenInGame(true);

	AbilityAimComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AbilityAimComponent"));
	AbilityAimComponent->SetupAttachment(RootComponent);
	AbilityAimComponent->SetHiddenInGame(true);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 90));
	SpringArmComponent->TargetArmLength = 500;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	CharacterMovementComponent = GetCharacterMovement();

	CharacterMovementComponent->MaxAcceleration = 20000;
	CharacterMovementComponent->MaxWalkSpeed = 5000;
	CharacterMovementComponent->JumpZVelocity = 1000;
	CharacterMovementComponent->AirControl = 1;
	CharacterMovementComponent->GravityScale = 3;

	BasicAttackTimerDelegate.BindUFunction(this, FName("NextBasicAttack"), false);
	PrefireTimerDelegate.BindUFunction(this, FName("OnPrefireTimerEnd"), false);
	JumpTimerDelegate.BindUFunction(this, FName("OnEndJump"), false);
	ProgressionResetTimerDelegate.BindUFunction(this, FName("ResetProgression"), false);
	FinishProgressionResetTimerDelegate.BindUFunction(this, FName("FinishResetProgression"), false);
}

bool ASLGod::GetBIsOrder() const
{
	return bIsOrder;
}

float ASLGod::GetPhysicalProtections() const
{
	return CurrentPhysicalProtections;
}

float ASLGod::GetMagicalProtections() const
{
	return CurrentMagicalProtections;
}

void ASLGod::SetMovementSpeed(float Val)
{
	UndiminishedMovementSpeed = Val;
	if (UndiminishedMovementSpeed >= MovementSpeedDiminishments[0])
	{
		if (UndiminishedMovementSpeed >= MovementSpeedDiminishments[1])
		{
			DiminishedMovementSpeed = MovementSpeedDiminishments[0] + (MovementSpeedDiminishments[1] - MovementSpeedDiminishments[0]) * MovementSpeedDiminishmentMultipliers[0] + (Val - MovementSpeedDiminishments[1]) * MovementSpeedDiminishmentMultipliers[1];
		}
		else
		{
			DiminishedMovementSpeed = MovementSpeedDiminishments[0] + (Val - MovementSpeedDiminishments[0]) * MovementSpeedDiminishmentMultipliers[0];
		}
	}
	else if (UndiminishedMovementSpeed < MinimumMovementSpeed)
	{
		UndiminishedMovementSpeed = MinimumMovementSpeed;
		DiminishedMovementSpeed = MinimumMovementSpeed;
	}
	else
	{
		DiminishedMovementSpeed = UndiminishedMovementSpeed;
	}
}

void ASLGod::SetBasicAttackSpeed(float Val)
{
	CurrentBasicAttackSpeed = Val;
}

void ASLGod::SetGodLevel(int Val)
{
	GodLevel = Val;
	Ability1Level = 0;
	Ability2Level = 0;
	Ability3Level = 0;
	Ability4Level = 0;
	SetBaseStatistics();
}

void ASLGod::SetBaseStatistics()
{
	SetMovementSpeed(BaseMovementSpeed);
	CurrentBasicAttackSpeed = BaseBasicAttackSpeed + (BaseBasicAttackSpeed * BasicAttackSpeedPerLevel) * GodLevel;
	CurrentBasicAttackDamage = BaseBasicAttackDamage + BasicAttackDamagePerLevel * GodLevel;
	MaxHealth = BaseHealth + HealthPerLevel * GodLevel;
	CurrentHealth = MaxHealth;
	CurrentPhysicalProtections = BasePhysicalProtections + PhysicalProtectionsPerLevel * GodLevel;
	CurrentMagicalProtections = BaseMagicalProtections + MagicalProtectionsPerLevel * GodLevel;
	CurrentHealthPerFive = BaseHealthPerFive + HealthPerFivePerLevel * GodLevel;
	AbilityPoints = GodLevel;
}

float ASLGod::GetCurrentBasicAttackDamage() const
{
	return CurrentBasicAttackDamage;
}

float ASLGod::GetPhysicalPower() const
{
	return PhysicalPower;
}

float ASLGod::GetMagicalPower() const
{
	return MagicalPower;
}

float ASLGod::GetFlatPhysicalPenetration() const
{
	return FlatPhysicalPenetration;
}

float ASLGod::GetFlatMagicalPenetration() const
{
	return FlatMagicalPenetration;
}

float ASLGod::GetPercentagePhysicalPenetration() const
{
	return PercentagePhysicalPenetration;
}

float ASLGod::GetPercentageMagicalPenetration() const
{
	return PercentageMagicalPenetration;
}

float ASLGod::GetBasicAttackPowerScaling() const
{
	return BasicAttackPowerScaling;
}

bool ASLGod::GetIsPhysicalDamage() const
{
	return bIsPhysicalDamage;
}

float ASLGod::CalculateTotalProtections(ISLVulnerable* Targeted) const
{
	if (bIsPhysicalDamage) return (Targeted->GetPhysicalProtections()) * (1 - GetPercentagePhysicalPenetration()) - GetFlatPhysicalPenetration() > 0 ? (Targeted->GetPhysicalProtections()) * (1 - GetPercentagePhysicalPenetration()) - GetFlatPhysicalPenetration() : 0;
	return (Targeted->GetMagicalProtections()) * (1 - GetPercentageMagicalPenetration()) - GetFlatMagicalPenetration() > 0 ? (Targeted->GetMagicalProtections()) * (1 - GetPercentageMagicalPenetration()) - GetFlatMagicalPenetration() : 0;
}

// Called when the game starts or when spawned
void ASLGod::BeginPlay()
{
	Super::BeginPlay();

	SetBaseStatistics();

	if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		PlayerController->SetControlRotation(FRotator(-35, 0, 0));

		RangeLineComponent->SetHiddenInGame(false);
		RangeLineComponent->SetRelativeLocation(FVector(0, 0, (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2));
		RangeLineComponent->SetRelativeScale3D(FVector(70, .5, .05));

		AbilityAimComponent->SetHiddenInGame(false);
		AbilityAimComponent->SetRelativeLocation(FVector(3700, 0, (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2));
		AbilityAimComponent->SetRelativeScale3D(FVector(4, 4, .1));
		
		ResetProgression();
	}
}

void ASLGod::LookUp(float Val)
{
	AddControllerPitchInput(-Val);

	//Old test code used to determine the Ability Aim Targeter position using the camera pitch, changed to use a raycast from the camera instead to match the game.
	/*
	if (PlayerController->GetControlRotation().Pitch >= UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMin)
	{
		float AbilityAimComponentPosition{ 0 };
		if (PlayerController->GetControlRotation().Pitch <= UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMin + 60)
		{
			AbilityAimComponentPosition = (PlayerController->GetControlRotation().Pitch - UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMin) / 60 * 6400 + 600;
		}
		else
		{
			AbilityAimComponentPosition = 7000;
		}
		AbilityAimComponent->SetRelativeLocation(FVector(AbilityAimComponentPosition, 0, AbilityAimComponent->GetRelativeLocation().Z));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("%s"), *PlayerController->GetControlRotation().ToString()));
	}
	*/
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, CameraComponent->GetComponentLocation(), CameraComponent->GetComponentLocation() + PlayerController->GetControlRotation().Vector() * 100000, ECollisionChannel::ECC_GameTraceChannel2);

	AbilityAimComponent->SetWorldLocation(FVector(HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, HitResult.ImpactPoint.Z + 5));
	if (AbilityAimComponent->GetRelativeLocation().X > 7000 || HitResult.ImpactPoint == FVector::ZeroVector) AbilityAimComponent->SetRelativeLocation(FVector(7000, 0, AbilityAimComponent->GetRelativeLocation().Z));
	AbilityAimComponent->SetRelativeScale3D(FVector(AbilityAimComponent->GetRelativeLocation().X / (7000 / 4) + 4, AbilityAimComponent->GetRelativeLocation().X / (7000 / 4) + 4, AbilityAimComponent->GetRelativeScale3D().Z));
}

void ASLGod::TurnRight(float Val)
{
	AddControllerYawInput(Val);
}

void ASLGod::MoveForward(float Val)
{
	if (Val != 0)
	{
		if (PlayerController->IsInputKeyDown(FKey("A")))
		{
			if (Val == 1)
			{
				MoveDiagonally(1, -1); return;
			}
			MoveDiagonally(-1, -1); return;
		}
		else if (PlayerController->IsInputKeyDown(FKey("D")))
		{
			if (Val == 1)
			{
				MoveDiagonally(1, 1); return;
			}
			MoveDiagonally(-1, 1); return;
		}
		if (BasicAttackPenalty < FMath::Abs(Val) && !bFatalis)
		{ 
			if (Val > 0) Val = BasicAttackPenalty * DiminishedMovementSpeed / MaximumDiminishedMovementSpeed; 
			else Val = -BasicAttackPenalty * DiminishedMovementSpeed / MaximumDiminishedMovementSpeed; 
		}
		else Val = Val * DiminishedMovementSpeed / MaximumDiminishedMovementSpeed;
		AddMovementInput(StaticMeshComponent->GetForwardVector(), Val);
	}
}

void ASLGod::MoveRight(float Val)
{
	if (Val != 0)
	{
		if (PlayerController->IsInputKeyDown(FKey("W")) || PlayerController->IsInputKeyDown(FKey("S"))) return;
		if (BasicAttackPenalty < StrafePenalty && !bFatalis)
		{
			if (Val > 0) Val = BasicAttackPenalty * DiminishedMovementSpeed / MaximumDiminishedMovementSpeed;
			else Val = -BasicAttackPenalty * DiminishedMovementSpeed / MaximumDiminishedMovementSpeed;
		}
		else Val = Val * DiminishedMovementSpeed / MaximumDiminishedMovementSpeed;
		AddMovementInput(StaticMeshComponent->GetRightVector(), Val);

	}
}

void ASLGod::MoveDiagonally(int ValX, int ValY)
{
	float Val{ 0 };
	if (BasicAttackPenalty < StrafePenalty && !bFatalis) Val = BasicAttackPenalty * DiminishedMovementSpeed / MaximumDiminishedMovementSpeed;
	else Val = 0.8 * DiminishedMovementSpeed / MaximumDiminishedMovementSpeed;
	FVector Vec = FVector(StaticMeshComponent->GetForwardVector() * ValX + StaticMeshComponent->GetRightVector() * ValY);
	Vec.Normalize();
	AddMovementInput(Vec, Val);
}

void ASLGod::UseAbility1()
{
	if (Ability1Level > 0)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(Ability1CooldownTimerHandle)) GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("Ability 1 is cooling down: %fs"), GetWorld()->GetTimerManager().GetTimerRemaining(Ability1CooldownTimerHandle)));
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, TEXT("Ability 1 used!"));
			GetWorld()->GetTimerManager().SetTimer(Ability1CooldownTimerHandle, Ability1Cooldowns[Ability1Level - 1] * (1 - CooldownReductionPercentage), false);
		}
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, TEXT("No points in Ability 1."));
}

void ASLGod::UseAbility2()
{
	if (Ability2Level > 0)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(Ability2CooldownTimerHandle)) GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("Ability 2 is cooling down: %fs"), GetWorld()->GetTimerManager().GetTimerRemaining(Ability2CooldownTimerHandle)));
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, TEXT("Ability 2 used!"));
			GetWorld()->GetTimerManager().SetTimer(Ability2CooldownTimerHandle, Ability2Cooldowns[Ability2Level - 1] * (1 - CooldownReductionPercentage), false);
		}
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, TEXT("No points in Ability 2."));
}

void ASLGod::UseAbility3()
{
	if (Ability3Level > 0)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(Ability3CooldownTimerHandle)) GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("Ability 3 is cooling down: %fs"), GetWorld()->GetTimerManager().GetTimerRemaining(Ability3CooldownTimerHandle)));
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, TEXT("Ability 3 used!"));
			GetWorld()->GetTimerManager().SetTimer(Ability3CooldownTimerHandle, Ability3Cooldowns[Ability3Level - 1] * (1 - CooldownReductionPercentage), false);
		}
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, TEXT("No points in Ability 3."));
}

void ASLGod::UseAbility4()
{
	if (Ability4Level > 0)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(Ability4CooldownTimerHandle)) GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("Ultimate Ability is cooling down: %fs"), GetWorld()->GetTimerManager().GetTimerRemaining(Ability4CooldownTimerHandle)));
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, TEXT("Ultimate Ability used!"));
			GetWorld()->GetTimerManager().SetTimer(Ability4CooldownTimerHandle, Ability4Cooldowns[Ability4Level - 1] * (1 - CooldownReductionPercentage), false);
		}
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, TEXT("No points in Ultimate Ability."));
}

void ASLGod::LevelAbility1()
{
	if (AbilityPoints > 0)
	{
		if (Ability1Level < 5)
		{
			if (Ability1Level < (float)GodLevel / 2)
			{
				++Ability1Level;
				--AbilityPoints;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("Ability 1 is now Level %i! You have %i Ability Points left."), Ability1Level, AbilityPoints));
			}
			else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Rank of Ability 1 [%i] isn't less than your God Level divided by 2 [%i/2], cannot be levelled."), Ability1Level, GodLevel));
		}
		else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ability 1 is already Max Rank."));
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Ability Points available."));
}

void ASLGod::LevelAbility2()
{
	if (AbilityPoints > 0)
	{
		if (Ability2Level < 5)
		{
			if (Ability2Level < (float)GodLevel / 2)
			{
				++Ability2Level;
				--AbilityPoints;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("Ability 2 is now Level %i! You have %i Ability Points left."), Ability2Level, AbilityPoints));
			}
			else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Rank of Ability 2 [%i] isn't less than your God Level divided by 2 [%i/2], cannot be levelled."), Ability2Level, GodLevel));
		}
		else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ability 2 is already Max Rank."));
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Ability Points available."));
}

void ASLGod::LevelAbility3()
{
	if (AbilityPoints > 0)
	{
		if (Ability3Level < 5)
		{
			if (Ability3Level < (float)GodLevel / 2)
			{
				++Ability3Level;
				--AbilityPoints;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("Ability 3 is now Level %i! You have %i Ability Points left."), Ability3Level, AbilityPoints));
			}
			else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Rank of Ability 3 [%i] isn't less than your God Level divided by 2 [%f], cannot be levelled."), Ability3Level, (float)GodLevel / 2));
		}
		else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ability 3 is already Max Rank."));
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Ability Points available."));
}

void ASLGod::LevelAbility4()
{
	if (AbilityPoints > 0)
	{
		if (Ability4Level < 5)
		{
			if (Ability4Level * 4 <= GodLevel - 5 || GodLevel == 20)
			{
				++Ability4Level;
				--AbilityPoints;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("Ultimate Ability is now Level %i! You have %i Ability Points left."), Ability4Level, AbilityPoints));
			}
			else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Rank of Ultimate Ability multiplied by 4 [%i] isn't less than or equal to your God Level minus 5 [%i] and your God Level isn't 20, cannot be levelled."), Ability4Level * 4, GodLevel - 5));
		}
		else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ultimate Ability is already Max Rank."));
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Ability Points available."));
}

void ASLGod::OnBeginJump()
{
	if (BasicAttackPenalty == 1)
	{
		Jump();
		bIsJumping = true;
		if (!GetWorld()->GetTimerManager().IsTimerActive(JumpTimerHandle)) GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, JumpTimerDelegate, 1, false);
	}
}

void ASLGod::OnEndJump()
{
	bIsJumping = false;
	if (bIsBasicAttacking) BeginFireBasicAttack();
}

void ASLGod::OnBeginFireBasicAttack()
{
	bIsBasicAttacking = true;
	if (!bIsJumping)
	{
		BeginFireBasicAttack();
	}
}

void ASLGod::OnEndFireBasicAttack()
{
	bIsBasicAttacking = false;
}

void ASLGod::BeginFireBasicAttack()
{
	if (bIsBasicAttacking && !bIsJumping && BasicAttackPenalty == 1)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProgressionResetTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, BasicAttackRefireProgression[CurrentProgression] / CurrentBasicAttackSpeed, false);
		if (bIsBasicAttackRangedProgression[CurrentProgression])
		{
			BasicAttackPenalty = BasicAttackRangedPenalty;
			CurrentAimComponent = RangedAimComponent;
			RangedAimComponent->SetMaterial(0, MTargeterWindup);
			if (bHasScalingPrefireProgression[CurrentProgression] && BasicAttackRefireProgression[CurrentProgression] > BasicAttackRefireProgression[CurrentProgression] / CurrentBasicAttackSpeed) GetWorld()->GetTimerManager().SetTimer(PrefireTimerHandle, PrefireTimerDelegate, BasicAttackPrefireProgression[CurrentProgression] * ((BasicAttackRefireProgression[CurrentProgression] / CurrentBasicAttackSpeed) / BasicAttackRefireProgression[CurrentProgression]), false);
			else GetWorld()->GetTimerManager().SetTimer(PrefireTimerHandle, PrefireTimerDelegate, BasicAttackPrefireProgression[CurrentProgression], false);
		}
		else
		{
			BasicAttackPenalty = BasicAttackMeleePenalty;
			CurrentAimComponent = MeleeAimComponent;
			MeleeAimComponent->SetMaterial(0, MTargeterWindup);
			if (bHasScalingPrefireProgression[CurrentProgression] && BasicAttackPrefireProgression[CurrentProgression] < CurrentBasicAttackSpeed) GetWorld()->GetTimerManager().SetTimer(PrefireTimerHandle, PrefireTimerDelegate, BasicAttackPrefireProgression[CurrentProgression] * BasicAttackPrefireProgression[CurrentProgression] / CurrentBasicAttackSpeed, false);
			else GetWorld()->GetTimerManager().SetTimer(PrefireTimerHandle, PrefireTimerDelegate, BasicAttackPrefireProgression[CurrentProgression], false);
		}
	}
	else if (bIsJumping)
	{
		bIsBasicAttacking = true;
	}
	else GetWorld()->GetTimerManager().SetTimer(ProgressionResetTimerHandle, ProgressionResetTimerDelegate, ProgressionResetTime, false);
}

void ASLGod::NextBasicAttack()
{
	CurrentAimComponent->SetMaterial(0, MTargeterStandby);
	if (CurrentProgression < BasicAttackRefireProgression.Num() - 1) CurrentProgression++;
	else CurrentProgression = 0;
	BasicAttackPenalty = 1;
	BeginFireBasicAttack();
}

void ASLGod::OnPrefireTimerEnd()
{
	if (bIsBasicAttackRangedProgression[CurrentProgression]) FireRangedBasicAttack();
	else FireMeleeBasicAttack();
}

void ASLGod::FireRangedBasicAttack()
{
	ASLRangedBasicProjectile* SpawnedRangedBasicProjectile = GetWorld()->SpawnActorDeferred<ASLRangedBasicProjectile>(RangedBasicProjectile, GetTransform());
	TArray<float> BasicAttackDisjoints{ BasicAttackDisjointProgression[CurrentProgression * 2] + (SpawnedRangedBasicProjectile->GetProjectileLength() * 100) / 2, BasicAttackDisjointProgression[CurrentProgression * 2 + 1] };
	SpawnedRangedBasicProjectile->SetBasicAttackDisjoints(BasicAttackDisjoints);
	SpawnedRangedBasicProjectile->SetOrigin(Cast<ISLDangerous>(this));
	SpawnedRangedBasicProjectile->SetDamageProgressionMultiplier(BasicAttackDamageProgression[CurrentProgression]);
	SpawnedRangedBasicProjectile->SetProjectileSpeed(RangedBasicAttackProjectileSpeedProgression[CurrentProgression]);
	SpawnedRangedBasicProjectile->SetProjectileRange(BasicAttackRangeProgression[CurrentProgression]);
	SpawnedRangedBasicProjectile->SetProjectileSize(RangedBasicAttackProjectileSizeProgression[CurrentProgression]);
	SpawnedRangedBasicProjectile->SetCleave(bCleaveProgression[CurrentProgression]);
	SpawnedRangedBasicProjectile->SetCleaveDamage(CleaveDamageProgression[CurrentProgression]);
	SpawnedRangedBasicProjectile->SetCleaveRange(RangedCleaveRangeProgression[CurrentProgression]);
	UGameplayStatics::FinishSpawningActor(SpawnedRangedBasicProjectile, SpawnedRangedBasicProjectile->GetTransform());
	ChangeBasicAttackTargeter();
	CurrentAimComponent->SetMaterial(0, MTargeterFiring);
}

void ASLGod::FireMeleeBasicAttack()
{
	// I know there's a cleaner and more efficient way of doing this since I'm current doing the check twice, but it will do for now, will do something about the repeating damage code later

	TArray<AActor*> OverlappingActors;
	MeleeAimComponent->GetOverlappingActors(OverlappingActors);
	float ShortestDistance{ -1 };
	ISLVulnerable* CurrentTarget{ nullptr };
	for (AActor* var : OverlappingActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("%s"), *var->GetName()));
		if (Cast<ISLVulnerable>(var) && var != this)
		{
			if (FVector::Dist2D(this->GetActorLocation(), var->GetActorLocation()) < ShortestDistance || ShortestDistance == -1)
			{
				FHitResult HitResult;
				if (!GetWorld()->LineTraceSingleByChannel(HitResult, this->GetActorLocation(), var->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1))
				{
					CurrentTarget = Cast<ISLVulnerable>(var);
					ShortestDistance = FVector::Dist(this->GetActorLocation(), var->GetActorLocation());
				}
				else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *HitResult.Actor->GetName()));
			}
		}
	}
	if (CurrentTarget)
	{
		if (GetIsPhysicalDamage())
		{
			CurrentTarget->TakeHealthDamage(((GetCurrentBasicAttackDamage() + GetPhysicalPower() * GetBasicAttackPowerScaling()) * BasicAttackDamageProgression[CurrentProgression]) * (100 / (CalculateTotalProtections(CurrentTarget) + 100)), this);
		}
		else
		{
			CurrentTarget->TakeHealthDamage(((GetCurrentBasicAttackDamage() + GetMagicalPower() * GetBasicAttackPowerScaling()) * BasicAttackDamageProgression[CurrentProgression]) * (100 / (CalculateTotalProtections(CurrentTarget) + 100)), this);
		}
		if (bCleaveProgression[CurrentProgression])
		{
			for (AActor* var : OverlappingActors)
			{
				if (Cast<ISLVulnerable>(var) && var != this && Cast<ISLVulnerable>(var) != CurrentTarget)
				{
					FHitResult HitResult;
					if (!GetWorld()->LineTraceSingleByChannel(HitResult, this->GetActorLocation(), var->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1))
					{
						ISLVulnerable* CleaveTarget = Cast<ISLVulnerable>(var);
						if (GetIsPhysicalDamage())
						{
							CleaveTarget->TakeHealthDamage(((GetCurrentBasicAttackDamage() + GetPhysicalPower() * GetBasicAttackPowerScaling()) * BasicAttackDamageProgression[CurrentProgression] * CleaveDamageProgression[CurrentProgression]) * (100 / (CalculateTotalProtections(CleaveTarget) + 100)), this);
						}
						else
						{
							CleaveTarget->TakeHealthDamage(((GetCurrentBasicAttackDamage() + GetMagicalPower() * GetBasicAttackPowerScaling()) * BasicAttackDamageProgression[CurrentProgression] * CleaveDamageProgression[CurrentProgression]) * (100 / (CalculateTotalProtections(CleaveTarget) + 100)), this);
						}
					}
				}
			}
		}
	}
	ChangeBasicAttackTargeter();
	CurrentAimComponent->SetMaterial(0, MTargeterFiring);
}

void ASLGod::ResetProgression()
{
	CurrentProgression = 0;
	if (bIsBasicAttackRangedProgression[CurrentProgression])
	{
		RangedAimComponent->SetHiddenInGame(false);
		RangedAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression], RangedBasicAttackProjectileSizeProgression[CurrentProgression], .05));
		RangedAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[CurrentProgression * 2], BasicAttackDisjointProgression[CurrentProgression * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2));
		MeleeAimComponent->SetHiddenInGame(true);
		CurrentAimComponent = RangedAimComponent;
	}
	else
	{
		MeleeAimComponent->SetHiddenInGame(false);
		MeleeAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression], BasicAttackRangeProgression[CurrentProgression], .025));
		MeleeAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[CurrentProgression * 2], BasicAttackDisjointProgression[CurrentProgression * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 2.5));
		RangedAimComponent->SetHiddenInGame(true);
		CurrentAimComponent = MeleeAimComponent;
	}
	if (bInitialProgressionReset) bInitialProgressionReset = false;
	else 
	{
		CurrentAimComponent->SetMaterial(0, MTargeterReset);
		GetWorld()->GetTimerManager().SetTimer(FinishProgressionResetTimerHandle, FinishProgressionResetTimerDelegate, 0.25, false);
	}
}

void ASLGod::FinishResetProgression()
{
	if (CurrentAimComponent->GetMaterial(0) == MTargeterReset)
	{
		CurrentAimComponent->SetMaterial(0, MTargeterStandby);
	}
}

void ASLGod::ChangeBasicAttackTargeter()
{
	if (CurrentProgression < bIsBasicAttackRangedProgression.Num() - 1)
	{
		if (bIsBasicAttackRangedProgression[CurrentProgression + 1]) { CurrentAimComponent = RangedAimComponent; RangedAimComponent->SetHiddenInGame(false); MeleeAimComponent->SetHiddenInGame(true); }
		else { CurrentAimComponent = MeleeAimComponent; RangedAimComponent->SetHiddenInGame(true); MeleeAimComponent->SetHiddenInGame(false); }
	}
	else if (bIsBasicAttackRangedProgression[0]) { CurrentAimComponent = RangedAimComponent; RangedAimComponent->SetHiddenInGame(false); MeleeAimComponent->SetHiddenInGame(true); }
	else { CurrentAimComponent = MeleeAimComponent; RangedAimComponent->SetHiddenInGame(true); MeleeAimComponent->SetHiddenInGame(false); }
	if (CurrentAimComponent == RangedAimComponent)
	{
		if (CurrentProgression < BasicAttackRefireProgression.Num() - 1)
		{
			CurrentAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression + 1], RangedBasicAttackProjectileSizeProgression[CurrentProgression + 1], .05));
			CurrentAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[(CurrentProgression + 1) * 2], BasicAttackDisjointProgression[(CurrentProgression + 1) * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2));
		}
		else
		{
			CurrentAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[0], RangedBasicAttackProjectileSizeProgression[0], .05));
			CurrentAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[0], BasicAttackDisjointProgression[1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2));
		}
	}
	else if (CurrentAimComponent == MeleeAimComponent)
	{
		if (CurrentProgression < BasicAttackRefireProgression.Num() - 1)
		{
			CurrentAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression + 1], BasicAttackRangeProgression[CurrentProgression + 1], .025));
			CurrentAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[(CurrentProgression + 1) * 2], BasicAttackDisjointProgression[(CurrentProgression + 1) * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 2.5));
		}
		else
		{
			CurrentAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[0], BasicAttackRangeProgression[0], .025));
			CurrentAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[0], BasicAttackDisjointProgression[1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 2.5));
		}
	}
}

void ASLGod::TakeHealthDamage(float Val, ISLDangerous* Origin)
{
	float OriginalHealth = CurrentHealth;
	CurrentHealth -= Val;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s dealt %f damage to %s (%f -> %f)"), *Cast<AActor>(Origin)->GetName(), Val, *this->GetName(), OriginalHealth, CurrentHealth));
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s has been slain by %s!"), *this->GetName(), *Cast<AActor>(Origin)->GetName()));
		Destroy();
	}
}

// Called to bind functionality to input
void ASLGod::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &ASLGod::LookUp);
	PlayerInputComponent->BindAxis("TurnRight", this, &ASLGod::TurnRight);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASLGod::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASLGod::MoveRight);

	PlayerInputComponent->BindAction("Ability1", IE_Released, this, &ASLGod::UseAbility1);
	PlayerInputComponent->BindAction("Ability2", IE_Released, this, &ASLGod::UseAbility2);
	PlayerInputComponent->BindAction("Ability3", IE_Released, this, &ASLGod::UseAbility3);
	PlayerInputComponent->BindAction("Ability4", IE_Released, this, &ASLGod::UseAbility4);

	PlayerInputComponent->BindAction("LevelAbility1", IE_Pressed, this, &ASLGod::LevelAbility1);
	PlayerInputComponent->BindAction("LevelAbility2", IE_Pressed, this, &ASLGod::LevelAbility2);
	PlayerInputComponent->BindAction("LevelAbility3", IE_Pressed, this, &ASLGod::LevelAbility3);
	PlayerInputComponent->BindAction("LevelAbility4", IE_Pressed, this, &ASLGod::LevelAbility4);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASLGod::OnBeginJump);
	PlayerInputComponent->BindAction("BasicAttack", IE_Pressed, this, &ASLGod::OnBeginFireBasicAttack);
	PlayerInputComponent->BindAction("BasicAttack", IE_Released, this, &ASLGod::OnEndFireBasicAttack);
}