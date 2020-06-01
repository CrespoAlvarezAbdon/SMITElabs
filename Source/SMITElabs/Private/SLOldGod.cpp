// Fill out your copyright notice in the Description page of Project Settings.


#include "SLOldGod.h"

// Sets default values
ASLOldGod::ASLOldGod()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 90));
	SpringArmComponent->TargetArmLength = 500;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment((SpringArmComponent));

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	BasicAttackTimerDelegate.BindUFunction(this, FName("SetIsBasicAttacking"), false);
}

// Called when the game starts or when spawned
void ASLOldGod::BeginPlay()
{
	Super::BeginPlay();

	SetMovementSpeed(UndiminishedMovementSpeed);
	SetBasicAttackSpeed(1);
}

// Called every frame
void ASLOldGod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController->IsInputKeyDown(EKeys::LeftMouseButton) && !bIsBasicAttacking && !GetCharacterMovement()->IsFalling())
	{
		bIsRangedBasicAttack = true;
		SetIsBasicAttacking(true);
		GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, 1 / BasicAttackSpeed, false);
	}
	else if (PlayerController->IsInputKeyDown(EKeys::RightMouseButton) && !bIsBasicAttacking && !GetCharacterMovement()->IsFalling())
	{
		bIsRangedBasicAttack = false;
		SetIsBasicAttacking(true);
		GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, 1 / BasicAttackSpeed, false);
	}
}

void ASLOldGod::LookUp(float val)
{
	AddControllerPitchInput(-val);
}

void ASLOldGod::TurnRight(float val)
{
	AddControllerYawInput(val);
}

void ASLOldGod::MoveForward(float val)
{
	if (val != 0)
	{
		if (PlayerController->IsInputKeyDown(FKey("A")))
		{
			if (val == 1)
			{
				MoveDiagonally(1, -1); return;
			}
			MoveDiagonally(-1, -1); return;
		}
		else if (PlayerController->IsInputKeyDown(FKey("D")))
		{
			if (val == 1)
			{
				MoveDiagonally(1, 1); return;
			}
			MoveDiagonally(-1, 1); return;
		}
		if ((PlayerController->IsInputKeyDown(EKeys::LeftMouseButton) || (bIsBasicAttacking && bIsRangedBasicAttack)) && !GetCharacterMovement()->IsFalling())
		{
			if (FMath::IsNearlyEqual(val, -0.6f, 0.01f)) val = -1;
			val = val * 0.5 * DiminishedMovementSpeed / 753.55;
			consoleColor = FColor::Purple;
			if (!bIsBasicAttacking)
			{
				bIsRangedBasicAttack = true;
				SetIsBasicAttacking(true);
				GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, 1 / BasicAttackSpeed, false);
			}
		}
		else if ((PlayerController->IsInputKeyDown(EKeys::RightMouseButton) || (bIsBasicAttacking && !bIsRangedBasicAttack)) && !GetCharacterMovement()->IsFalling())
		{
			if (FMath::IsNearlyEqual(val, -0.6f, 0.1f)) val = val * DiminishedMovementSpeed / 753.55;
			else val = val * 0.65 * DiminishedMovementSpeed / 753.55;
			consoleColor = FColor::Red;
			if (!bIsBasicAttacking)
			{
				bIsRangedBasicAttack = false;
				SetIsBasicAttacking(true);
				GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, 1 / BasicAttackSpeed, false);
			}
		}
		else
		{
			val = val * DiminishedMovementSpeed / 753.55;
			consoleColor = FColor::Green;
		}
		AddMovementInput(StaticMeshComponent->GetForwardVector(), val);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, consoleColor, FString::Printf(TEXT("%s"), *GetVelocity().ToString()));
	}
}

void ASLOldGod::MoveRight(float val)
{
	if (val != 0)
	{
		if (PlayerController->IsInputKeyDown(FKey("W")) || PlayerController->IsInputKeyDown(FKey("S"))) return;
		if ((PlayerController->IsInputKeyDown(EKeys::LeftMouseButton) || (bIsBasicAttacking && bIsRangedBasicAttack)) && !GetCharacterMovement()->IsFalling())
		{
			val = val * 1.25 * 0.5 * DiminishedMovementSpeed / 753.55;
			consoleColor = FColor::Purple;
			if (!bIsBasicAttacking)
			{
				bIsRangedBasicAttack = true;
				SetIsBasicAttacking(true);
				GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, 1 / BasicAttackSpeed, false);
			}
		}
		else if ((PlayerController->IsInputKeyDown(EKeys::RightMouseButton) || (bIsBasicAttacking && !bIsRangedBasicAttack)) && !GetCharacterMovement()->IsFalling())
		{
			val = val * 1.25 * 0.65 * DiminishedMovementSpeed / 753.55;
			consoleColor = FColor::Red;
			if (!bIsBasicAttacking)
			{
				bIsRangedBasicAttack = false;
				SetIsBasicAttacking(true);
				GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, 1 / BasicAttackSpeed, false);
			}
		}
		else
		{
			val = val * DiminishedMovementSpeed / 753.55;
			consoleColor = FColor::Green;
		}
		AddMovementInput(StaticMeshComponent->GetRightVector(), val);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, consoleColor, FString::Printf(TEXT("%s"), *GetVelocity().ToString()));
	}
}

void ASLOldGod::MoveDiagonally(int valX, int valY)
{
	float val;
	if ((PlayerController->IsInputKeyDown(EKeys::LeftMouseButton) || (bIsBasicAttacking && bIsRangedBasicAttack)) && !GetCharacterMovement()->IsFalling())
	{
		val = 0.5 * DiminishedMovementSpeed / 753.55;
		consoleColor = FColor::Purple;
		if (!bIsBasicAttacking)
		{
			bIsRangedBasicAttack = true;
			SetIsBasicAttacking(true);
			GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, 1 / BasicAttackSpeed, false);
		}
	}
	else if ((PlayerController->IsInputKeyDown(EKeys::RightMouseButton) || (bIsBasicAttacking && !bIsRangedBasicAttack)) && !GetCharacterMovement()->IsFalling())
	{
		val = 0.65 * DiminishedMovementSpeed / 753.55;
		consoleColor = FColor::Red;
		if (!bIsBasicAttacking)
		{
			bIsRangedBasicAttack = false;
			SetIsBasicAttacking(true);
			GetWorld()->GetTimerManager().SetTimer(BasicAttackTimerHandle, BasicAttackTimerDelegate, 1 / BasicAttackSpeed, false);
		}
	}
	else
	{
		val = 0.8 * DiminishedMovementSpeed / 753.55;
		consoleColor = FColor::Green;
	}
	FVector vec = FVector(StaticMeshComponent->GetForwardVector() * valX + StaticMeshComponent->GetRightVector() * valY);
	vec.Normalize();
	AddMovementInput(vec, val);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, consoleColor, FString::Printf(TEXT("%s"), *GetVelocity().ToString()));
}

void ASLOldGod::SetIsBasicAttacking(bool val)
{
	bIsBasicAttacking = val;
	if (val)
	{
		if (bIsRangedBasicAttack) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Ranged Basic Attack Fired!"));
		else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Melee Basic Attack Fired!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Basic Attack Ended."));
	}
}

void ASLOldGod::SetMovementSpeed(float val)
{
	UndiminishedMovementSpeed = val;
	if (UndiminishedMovementSpeed >= 457)
	{
		if (UndiminishedMovementSpeed >= 540.5)
		{
			DiminishedMovementSpeed = 523.8 + (val - 540.5) * 0.5;
		}
		else
		{
			DiminishedMovementSpeed = 457 + (val - 457) * 0.8;
		}
	}
	else if (UndiminishedMovementSpeed < 150)
	{
		DiminishedMovementSpeed = 150;
	}
	else
	{
		DiminishedMovementSpeed = UndiminishedMovementSpeed;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Undiminished MS: %f, Diminished MS: %f"), UndiminishedMovementSpeed, DiminishedMovementSpeed));
}

void ASLOldGod::SetBasicAttackSpeed(float val)
{
	if (val > 2.5) BasicAttackSpeed = 2.5;
	else if (val < 0.01) BasicAttackSpeed = 0.01;
	else BasicAttackSpeed = val;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Basic Attack Speed: %f Attacks Per Second"), BasicAttackSpeed));
}

void ASLOldGod::OnJump()
{
	if (!bIsBasicAttacking) Jump();
}

// Called to bind functionality to input
void ASLOldGod::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &ASLOldGod::LookUp);
	PlayerInputComponent->BindAxis("TurnRight", this, &ASLOldGod::TurnRight);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASLOldGod::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASLOldGod::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASLOldGod::OnJump);
}