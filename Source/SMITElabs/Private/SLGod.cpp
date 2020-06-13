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

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 90));
	SpringArmComponent->TargetArmLength = 500;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	CharacterMovementComponent = GetCharacterMovement();

	CharacterMovementComponent->MaxAcceleration = 12000;
	CharacterMovementComponent->MaxWalkSpeed = 3000;
	CharacterMovementComponent->JumpZVelocity = 1000;
	CharacterMovementComponent->AirControl = 1;
	CharacterMovementComponent->GravityScale = 3;

	BasicAttackTimerDelegate.BindUFunction(this, FName("NextBasicAttack"), false);
	PrefireTimerDelegate.BindUFunction(this, FName("OnPrefireTimerEnd"), false);
	JumpTimerDelegate.BindUFunction(this, FName("OnEndJump"), false);
	ProgressionResetTimerDelegate.BindUFunction(this, FName("ResetProgression"), false);
}

// Called when the game starts or when spawned
void ASLGod::BeginPlay()
{
	Super::BeginPlay();

	SetMovementSpeed(BaseMovementSpeed);
	CurrentBasicAttackDamage = BaseBasicAttackDamage;
	
	if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		RangeLineComponent->SetHiddenInGame(false);
		RangeLineComponent->SetRelativeLocation(FVector(0, 0, (StaticMeshComponent->GetRelativeScale3D().Z * 100) / -2));
		RangeLineComponent->SetRelativeScale3D(FVector(70, .5, .05));
		
		ResetProgression();
	}
}

void ASLGod::LookUp(float Val)
{
	AddControllerPitchInput(-Val);
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
		if (BasicAttackPenalty < BasicAttackStrafePenalty && !bFatalis)
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
	if (BasicAttackPenalty < BasicAttackStrafePenalty && !bFatalis) Val = BasicAttackPenalty * DiminishedMovementSpeed / MaximumDiminishedMovementSpeed;
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
		GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, JumpTimerDelegate, 1, false);
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
		GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, BasicAttackRefireProgression[CurrentProgression] / BasicAttackSpeed, false);
		if (bIsBasicAttackRangedProgression[CurrentProgression])
		{
			BasicAttackPenalty = BasicAttackRangedPenalty;
			CurrentAimComponent = RangedAimComponent;
			RangedAimComponent->SetMaterial(0, MTargeterWindup);
			if (bHasScalingPrefireProgression[CurrentProgression] && BasicAttackRefireProgression[CurrentProgression] > BasicAttackRefireProgression[CurrentProgression] / BasicAttackSpeed) GetWorld()->GetTimerManager().SetTimer(PrefireTimerHandle, PrefireTimerDelegate, BasicAttackPrefireProgression[CurrentProgression] * ((BasicAttackRefireProgression[CurrentProgression] / BasicAttackSpeed) / BasicAttackRefireProgression[CurrentProgression]), false);
			else GetWorld()->GetTimerManager().SetTimer(PrefireTimerHandle, PrefireTimerDelegate, BasicAttackPrefireProgression[CurrentProgression], false);
		}
		else
		{
			BasicAttackPenalty = BasicAttackMeleePenalty;
			CurrentAimComponent = MeleeAimComponent;
			MeleeAimComponent->SetMaterial(0, MTargeterWindup);
			if (bHasScalingPrefireProgression[CurrentProgression] && BasicAttackPrefireProgression[CurrentProgression] < BasicAttackSpeed) GetWorld()->GetTimerManager().SetTimer(PrefireTimerHandle, PrefireTimerDelegate, BasicAttackPrefireProgression[CurrentProgression] * BasicAttackPrefireProgression[CurrentProgression] / BasicAttackSpeed, false);
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
	TArray<float> BasicAttackDisjoints{ BasicAttackDisjointProgression[CurrentProgression * 2], BasicAttackDisjointProgression[CurrentProgression * 2 + 1] };
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
			float TotalProtections = (CurrentTarget->GetPhysicalProtections()) * (1 - GetPercentagePhysicalPenetration()) - GetFlatPhysicalPenetration() > 0 ? (CurrentTarget->GetPhysicalProtections()) * (1 - GetPercentagePhysicalPenetration()) - GetFlatPhysicalPenetration() : 0;
			CurrentTarget->TakeHealthDamage(((GetCurrentBasicAttackDamage() + GetPhysicalPower() * GetBasicAttackPowerScaling()) * BasicAttackDamageProgression[CurrentProgression]) * (100 / (TotalProtections + 100)), this);
		}
		else
		{
			float TotalProtections = (CurrentTarget->GetMagicalProtections()) * (1 - GetPercentageMagicalPenetration()) - GetFlatMagicalPenetration() > 0 ? (CurrentTarget->GetMagicalProtections()) * (1 - GetPercentageMagicalPenetration()) - GetFlatMagicalPenetration() : 0;
			CurrentTarget->TakeHealthDamage(((GetCurrentBasicAttackDamage() + GetMagicalPower() * GetBasicAttackPowerScaling()) * BasicAttackDamageProgression[CurrentProgression]) * (100 / (TotalProtections + 100)), this);
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
							float TotalProtections = (CleaveTarget->GetPhysicalProtections()) * (1 - GetPercentagePhysicalPenetration()) - GetFlatPhysicalPenetration() > 0 ? (CleaveTarget->GetPhysicalProtections()) * (1 - GetPercentagePhysicalPenetration()) - GetFlatPhysicalPenetration() : 0;
							CleaveTarget->TakeHealthDamage(((GetCurrentBasicAttackDamage() + GetPhysicalPower() * GetBasicAttackPowerScaling()) * BasicAttackDamageProgression[CurrentProgression] * CleaveDamageProgression[CurrentProgression]) * (100 / (TotalProtections + 100)), this);
						}
						else
						{
							float TotalProtections = (CleaveTarget->GetMagicalProtections()) * (1 - GetPercentageMagicalPenetration()) - GetFlatMagicalPenetration() > 0 ? (CleaveTarget->GetMagicalProtections()) * (1 - GetPercentageMagicalPenetration()) - GetFlatMagicalPenetration() : 0;
							CleaveTarget->TakeHealthDamage(((GetCurrentBasicAttackDamage() + GetMagicalPower() * GetBasicAttackPowerScaling()) * BasicAttackDamageProgression[CurrentProgression] * CleaveDamageProgression[CurrentProgression]) * (100 / (TotalProtections + 100)), this);
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, ConsoleColor, FString::Printf(TEXT("Progression Reset: No AA After %fs"), ProgressionResetTime));
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
	CurrentAimComponent->SetMaterial(0, MTargeterStandby);
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
	ISLVulnerable::TakeHealthDamage(Val, Origin);
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