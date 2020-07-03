// Fill out your copyright notice in the Description page of Project Settings.


#include "SLGod.h"
#include "SMITElabs/Public/SLRangedBasicProjectile.h"

// Sets default values
ASLGod::ASLGod()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(312.5);
	GetCapsuleComponent()->SetCapsuleRadius(156.25);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetRelativeScale3D(FVector(3.125, 3.125, 6.250));
	StaticMeshComponent->SetupAttachment(RootComponent);

	RangeLineComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RangeLineComponent"));
	RangeLineComponent->SetRelativeLocation(FVector(0, 0, (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
	RangeLineComponent->SetRelativeScale3D(FVector(70, .5, .1));
	RangeLineComponent->SetupAttachment(RootComponent);
	RangeLineComponent->SetVisibility(false);

	RangedAimComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RangedAimComponent"));
	RangedAimComponent->SetupAttachment(RootComponent);
	RangedAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[CurrentProgression * 2], BasicAttackDisjointProgression[CurrentProgression * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
	RangedAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression], RangedBasicAttackProjectileSizeProgression[CurrentProgression], .1));
	RangedAimComponent->SetVisibility(false);

	MeleeAimComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeAimComponent"));
	MeleeAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[CurrentProgression * 2], BasicAttackDisjointProgression[CurrentProgression * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
	MeleeAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression], BasicAttackRangeProgression[CurrentProgression], .1));
	MeleeAimComponent->SetupAttachment(RootComponent);
	MeleeAimComponent->SetVisibility(false);

	TargeterLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TargeterLocationComponent"));
	TargeterLocationComponent->SetupAttachment(RootComponent);
	TargeterLocationComponent->SetRelativeLocation(FVector(3700, 0, (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
	TargeterLocationComponent->SetVisibility(false);

	AbilityAimComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AbilityAimComponent"));
	AbilityAimComponent->SetupAttachment(TargeterLocationComponent);
	AbilityAimComponent->SetRelativeScale3D(FVector(4, 4, .1));
	AbilityAimComponent->SetVisibility(false);	

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
	PerFiveTimerDelegate.BindUFunction(this, FName("SustainPerFive"), false);
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
	SetBaseStatistics();
}

void ASLGod::SetBaseStatistics()
{
	SetMovementSpeed(BaseMovementSpeed + MovementSpeedPerLevel * GodLevel);
	CurrentBasicAttackSpeed = BaseBasicAttackSpeed + (BaseBasicAttackSpeed * BasicAttackSpeedPerLevel) * GodLevel;
	CurrentBasicAttackDamage = BaseBasicAttackDamage + BasicAttackDamagePerLevel * GodLevel;
	MaxHealth = BaseHealth + HealthPerLevel * GodLevel;
	CurrentHealth = MaxHealth;
	MaxMana = BaseMana + ManaPerLevel * GodLevel;
	CurrentMana = MaxMana;
	CurrentPhysicalProtections = BasePhysicalProtections + PhysicalProtectionsPerLevel * GodLevel;
	CurrentMagicalProtections = BaseMagicalProtections + MagicalProtectionsPerLevel * GodLevel;
	CurrentHealthPerFive = BaseHealthPerFive + HealthPerFivePerLevel * GodLevel;
	CurrentManaPerFive = BaseManaPerFive + ManaPerFivePerLevel * GodLevel;
	AbilityPoints = GodLevel;

	if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("MP5: %f"), CurrentManaPerFive));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("HP5: %f"), CurrentHealthPerFive));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Magical Protections: %f"), CurrentMagicalProtections));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Physical Protections: %f"), CurrentPhysicalProtections));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Max Mana: %f"), MaxMana));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Max Health: %f"), MaxHealth));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Basic Attack Damage: %f"), CurrentBasicAttackDamage));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Basic Attack Speed: %f"), CurrentBasicAttackSpeed));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Undimimished Movement Speed: %f, Diminished Movement Speed: %f"), UndiminishedMovementSpeed, DiminishedMovementSpeed));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("%s Level %i"), *UnitName, GodLevel));
	}
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
		RangeLineComponent->SetVisibility(true);
		AbilityAimComponent->SetVisibility(true);
		
		ResetProgression();
	}

	GetWorld()->GetTimerManager().SetTimer(PerFiveTimerHandle, PerFiveTimerDelegate, 1, true);

	int j = 0;
	int k = 0;
	for (int i = 0; i < NumberOfAbilities; i++)
	{
		k += ATCCount[i];
		int m = j - 1;
		for (; j < k; j++)
		{
			AbilityTargeterComponents.Add(NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("%s Targeter Component [%i]"), *AbilityNames[i], j - m)));
			AbilityTargeterComponents[j]->RegisterComponent();
			AbilityTargeterComponents[j]->SetUsingAbsoluteScale(true);
			FAttachmentTransformRules ATR = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
			if (bFollowGroundTargeter[j])
			{
				AbilityTargeterComponents[j]->AttachToComponent(TargeterLocationComponent, ATR);
				AbilityTargeterComponents[j]->SetRelativeLocation(FVector(AbilityTargeterPositionsX[j], AbilityTargeterPositionsY[j], 0));
			}
			else
			{
				AbilityTargeterComponents[j]->AttachToComponent(RootComponent, ATR);
				AbilityTargeterComponents[j]->SetRelativeLocation(FVector(AbilityTargeterPositionsX[j], AbilityTargeterPositionsY[j], -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 5));
			}
			AbilityTargeterComponents[j]->SetWorldScale3D(FVector(AbilityTargeterScalesX[j], AbilityTargeterScalesY[j], 0.1));
			AbilityTargeterComponents[j]->SetRelativeRotation(FRotator(0, AbilityTargeterRotations[j], 0));
			AbilityTargeterComponents[j]->SetStaticMesh(AbilityTargeterMeshes[j]);
			AbilityTargeterComponents[j]->SetMaterial(0, MAbilityTargeter);
			AbilityTargeterComponents[j]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ASLGod::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName().ToString() == "NumberOfAbilities")
	{
		SetAbilityArrays();
		SetAbilityTargeterArrays();
	}
	else if (PropertyChangedEvent.GetPropertyName().ToString() == "ATCCount") SetAbilityTargeterArrays();

	if (AbilityTargeterComponents.Num() > 0)
	{
		if (PropertyChangedEvent.GetPropertyName().ToString() == "AbilityTargeterScalesX")
		{
			int i = PropertyChangedEvent.GetArrayIndex("AbilityTargeterScalesX");
			AbilityTargeterComponents[i]->SetWorldScale3D(FVector(AbilityTargeterScalesX[i], AbilityTargeterScalesY[i], 0.1));
		}
		else if (PropertyChangedEvent.GetPropertyName().ToString() == "AbilityTargeterScalesY")
		{
			int i = PropertyChangedEvent.GetArrayIndex("AbilityTargeterScalesY");
			AbilityTargeterComponents[i]->SetWorldScale3D(FVector(AbilityTargeterScalesX[i], AbilityTargeterScalesY[i], 0.1));
		}
		else if (PropertyChangedEvent.GetPropertyName().ToString() == "AbilityTargeterPositionsX")
		{
			int i = PropertyChangedEvent.GetArrayIndex("AbilityTargeterPositionsX");
			if (bFollowGroundTargeter[i])
				AbilityTargeterComponents[i]->SetRelativeLocation(FVector(AbilityTargeterPositionsX[i], AbilityTargeterPositionsY[i], 0));
			else
				AbilityTargeterComponents[i]->SetRelativeLocation(FVector(AbilityTargeterPositionsX[i], AbilityTargeterPositionsY[i], -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 5));
		}
		else if (PropertyChangedEvent.GetPropertyName().ToString() == "AbilityTargeterPositionsY")
		{
			int i = PropertyChangedEvent.GetArrayIndex("AbilityTargeterPositionsY");
			if (bFollowGroundTargeter[i])
				AbilityTargeterComponents[i]->SetRelativeLocation(FVector(AbilityTargeterPositionsX[i], AbilityTargeterPositionsY[i], 0));
			else
				AbilityTargeterComponents[i]->SetRelativeLocation(FVector(AbilityTargeterPositionsX[i], AbilityTargeterPositionsY[i], -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 5));
		}
		else if (PropertyChangedEvent.GetPropertyName().ToString() == "AbilityTargeterRotations")
		{
			int i = PropertyChangedEvent.GetArrayIndex("AbilityTargeterRotations");
			AbilityTargeterComponents[i]->SetRelativeRotation(FRotator(0, AbilityTargeterRotations[i], 0));
		}
		else if (PropertyChangedEvent.GetPropertyName().ToString() == "bFollowGroundTargeter")
		{
			FAttachmentTransformRules ATR = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
			int i = PropertyChangedEvent.GetArrayIndex("bFollowGroundTargeter");
			if (bFollowGroundTargeter[i])
			{
				AbilityTargeterComponents[i]->AttachToComponent(TargeterLocationComponent, ATR);
				AbilityTargeterComponents[i]->SetRelativeLocation(FVector(AbilityTargeterPositionsX[i], AbilityTargeterPositionsY[i], 0));

			}
			else
			{
				AbilityTargeterComponents[i]->AttachToComponent(RootComponent, ATR);
				AbilityTargeterComponents[i]->SetRelativeLocation(FVector(AbilityTargeterPositionsX[i], AbilityTargeterPositionsY[i], -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 5));
			}
		}
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

	TargeterLocationComponent->SetWorldLocation(FVector(HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, HitResult.ImpactPoint.Z + 5));
	if (TargeterLocationComponent->GetRelativeLocation().X > CurrentMaxTargeterRange || HitResult.ImpactPoint == FVector::ZeroVector) TargeterLocationComponent->SetRelativeLocation(FVector(CurrentMaxTargeterRange, 0, TargeterLocationComponent->GetRelativeLocation().Z));
	AbilityAimComponent->SetRelativeScale3D(FVector(TargeterLocationComponent->GetRelativeLocation().X / (7000 / 4) + 4, TargeterLocationComponent->GetRelativeLocation().X / (7000 / 4) + 4, AbilityAimComponent->GetRelativeScale3D().Z));
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
	TArray<AActor*> OverlappingActors;
	MeleeAimComponent->GetOverlappingActors(OverlappingActors);
	float ShortestDistance{ -1 };
	ISLVulnerable* CurrentTarget{ nullptr };
	for (AActor* var : OverlappingActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("%s"), *var->GetName()));
		if (Cast<ISLVulnerable>(var) && var != this && Cast<ISLIdentifiable>(var)->GetBIsOrder() != bIsOrder)
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
				if (Cast<ISLVulnerable>(var) && var != this && Cast<ISLVulnerable>(var) != CurrentTarget && Cast<ISLIdentifiable>(var)->GetBIsOrder() != bIsOrder)
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
		RangedAimComponent->SetVisibility(true);
		RangedAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression], RangedBasicAttackProjectileSizeProgression[CurrentProgression], .1));
		RangedAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[CurrentProgression * 2], BasicAttackDisjointProgression[CurrentProgression * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
		MeleeAimComponent->SetVisibility(false);
		CurrentAimComponent = RangedAimComponent;
	}
	else
	{
		MeleeAimComponent->SetVisibility(true);
		MeleeAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression], BasicAttackRangeProgression[CurrentProgression], .1));
		MeleeAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[CurrentProgression * 2], BasicAttackDisjointProgression[CurrentProgression * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
		RangedAimComponent->SetVisibility(false);
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
		if (bIsBasicAttackRangedProgression[CurrentProgression + 1]) { CurrentAimComponent = RangedAimComponent; RangedAimComponent->SetVisibility(true); MeleeAimComponent->SetVisibility(false); }
		else { CurrentAimComponent = MeleeAimComponent; RangedAimComponent->SetVisibility(false); MeleeAimComponent->SetVisibility(true); }
	}
	else if (bIsBasicAttackRangedProgression[0]) { CurrentAimComponent = RangedAimComponent; RangedAimComponent->SetVisibility(true); MeleeAimComponent->SetVisibility(false); }
	else { CurrentAimComponent = MeleeAimComponent; RangedAimComponent->SetVisibility(false); MeleeAimComponent->SetVisibility(true); }
	if (CurrentAimComponent == RangedAimComponent)
	{
		if (CurrentProgression < BasicAttackRefireProgression.Num() - 1)
		{
			CurrentAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression + 1], RangedBasicAttackProjectileSizeProgression[CurrentProgression + 1], .1));
			CurrentAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[(CurrentProgression + 1) * 2], BasicAttackDisjointProgression[(CurrentProgression + 1) * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
		}
		else
		{
			CurrentAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[0], RangedBasicAttackProjectileSizeProgression[0], .1));
			CurrentAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[0], BasicAttackDisjointProgression[1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
		}
	}
	else if (CurrentAimComponent == MeleeAimComponent)
	{
		if (CurrentProgression < BasicAttackRefireProgression.Num() - 1)
		{
			CurrentAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[CurrentProgression + 1], BasicAttackRangeProgression[CurrentProgression + 1], .1));
			CurrentAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[(CurrentProgression + 1) * 2], BasicAttackDisjointProgression[(CurrentProgression + 1) * 2 + 1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
		}
		else
		{
			CurrentAimComponent->SetRelativeScale3D(FVector(BasicAttackRangeProgression[0], BasicAttackRangeProgression[0], .1));
			CurrentAimComponent->SetRelativeLocation(FVector(BasicAttackDisjointProgression[0], BasicAttackDisjointProgression[1], (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2 + 5));
		}
	}
}

void ASLGod::SustainPerFive()
{
	if (CurrentHealth + CurrentHealthPerFive / 5 <= MaxHealth) CurrentHealth += CurrentHealthPerFive / 5;
	else if (CurrentHealth != MaxHealth) CurrentHealth = MaxHealth;
	if (CurrentMana + CurrentManaPerFive / 5 <= MaxMana) CurrentMana += CurrentManaPerFive / 5;
	else if (CurrentMana != MaxMana) CurrentMana = MaxMana;
}


void ASLGod::SetAbilityArrays()
{
	AbilityCooldownTimerHandles.SetNum(NumberOfAbilities, true);
	AbilityCooldownTimerDelegates.SetNum(NumberOfAbilities, true);
	AbilityNames.SetNum(NumberOfAbilities, true);
	AbilityCharges.SetNum(NumberOfAbilities, true);
	AbilityRankOneCooldowns.SetNum(NumberOfAbilities, true);
	AbilityRankTwoCooldowns.SetNum(NumberOfAbilities, true);
	AbilityRankThreeCooldowns.SetNum(NumberOfAbilities, true);
	AbilityRankFourCooldowns.SetNum(NumberOfAbilities, true);
	AbilityRankFiveCooldowns.SetNum(NumberOfAbilities, true);
	AbilityRankOneManaCosts.SetNum(NumberOfAbilities, true);
	AbilityRankTwoManaCosts.SetNum(NumberOfAbilities, true);
	AbilityRankThreeManaCosts.SetNum(NumberOfAbilities, true);
	AbilityRankFourManaCosts.SetNum(NumberOfAbilities, true);
	AbilityRankFiveManaCosts.SetNum(NumberOfAbilities, true);
	bAreInstantCast.SetNum(NumberOfAbilities, true);
	ATCCount.SetNum(NumberOfAbilities, true);
}

void ASLGod::SetAbilityTargeterArrays()
{
	int ATCCountTotal = 0;
	for (int i : ATCCount) ATCCountTotal += i;
	AbilityTargeterMeshes.SetNum(ATCCountTotal, true);
	AbilityTargeterScalesX.SetNum(ATCCountTotal, true);
	AbilityTargeterScalesY.SetNum(ATCCountTotal, true);
	AbilityTargeterPositionsX.SetNum(ATCCountTotal, true);
	AbilityTargeterPositionsY.SetNum(ATCCountTotal, true);
	AbilityTargeterRotations.SetNum(ATCCountTotal, true);
	bFollowGroundTargeter.SetNum(ATCCountTotal, true);
}

void ASLGod::OnBasicAttackHit(TArray<ISLVulnerable*> Targets)
{
	return;
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASLGod::OnBeginJump);
	PlayerInputComponent->BindAction("BasicAttack", IE_Pressed, this, &ASLGod::OnBeginFireBasicAttack);
	PlayerInputComponent->BindAction("BasicAttack", IE_Released, this, &ASLGod::OnEndFireBasicAttack);
}