// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SMITElabs/Public/SLDamageable.h"
#include "SLGod.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UGameplayStatics;
class UPrimitiveComponent;
class UArrowComponent;
class APlayerController;
class UCharacterMovementComponent;
class FTimerManager;
class ASLRangedBasicProjectile;
class ISLDamageable;
struct FTimerHandle;

/**
 *
 */
UCLASS(Abstract)
class SMITELABS_API ASLGod : public ACharacter, public ISLDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASLGod();

	virtual void SetCurrentHealth(float Val, AActor* Origin) override;

	void SetMovementSpeed(float Val);

	void SetBasicAttackSpeed(float Val);

	float GetCurrentBasicAttackDamage() const;

	float GetPhysicalPower() const;

	float GetMagicalPower() const;

	float GetFlatPhysicalPenetration() const;

	float GetFlatMagicalPenetration() const;

	float GetPercentagePhysicalPenetration() const;

	float GetPercentageMagicalPenetration() const;

	float GetBasicAttackPowerScaling() const;

	bool GetIsPhysicalDamage() const;

	void LookUp(float Val);

	void TurnRight(float Val);

	void MoveForward(float Val);

	void MoveRight(float Val);

	void MoveDiagonally(int ValX, int ValY);

	void OnBeginJump();

	UFUNCTION()
	void OnEndJump();

	void OnBeginFireBasicAttack();

	void OnEndFireBasicAttack();

	void BeginFireBasicAttack();

	UFUNCTION()
	void NextBasicAttack();

	UFUNCTION()
	void OnRangedPrefireTimerEnd();
	
	void FireRangedBasicAttack();

	UFUNCTION()
	void OnMeleePrefireTimerEnd();

	void FireMeleeBasicAttack();

	UFUNCTION()
	void ResetProgression();

	void ChangeBasicAttackTargeter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RangeLineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RangedAimComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeleeAimComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ASLRangedBasicProjectile> RangedBasicProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MTargeterStandby;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MTargeterWindup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MTargeterFiring;

	UStaticMeshComponent* CurrentAimComponent;

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float BaseMovementSpeed{ 360 };

	float UndiminishedMovementSpeed{ 0 };

	float DiminishedMovementSpeed{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float BasicAttackSpeed{ 1 };

	bool bIsBasicAttacking{ false };

	bool bIsJumping{ false };

	bool bFatalis{ false };

	float BasicAttackPenalty{ 1 };

	UPROPERTY(EditAnywhere, Category = "Statistics", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float BaseBasicAttackDamage{ 43 };

	UPROPERTY(EditAnywhere, Category = "Statistics", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float LevelBasicAttackDamage{ 1 };

	float CurrentBasicAttackDamage{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float BasicAttackPowerScaling{ 1 };

	UPROPERTY(EditAnywhere, Category = "Statistics", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float PhysicalPower{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MagicalPower{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics", meta=(ClampMin = "0.0", UIMin = "0.0", ClampMax = "50.0", UIMax = "50.0"))
	float FlatPhysicalPenetration{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "50.0", UIMax = "50.0"))
	float FlatMagicalPenetration{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "0.4", UIMax = "0.4"))
	float PercentagePhysicalPenetration{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "0.4", UIMax = "0.4"))
	float PercentageMagicalPenetration{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	bool bIsPhysicalDamage{ true };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<float> BasicAttackRefireProgression{ 0.5, 1, 1 };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<float> BasicAttackPrefireProgression{ 0.25, 0.25, 0.5 };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<float> BasicAttackRangeProgression{ 55, 55, 55 };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<float> BasicAttackDamageProgression{ BasicAttackRefireProgression };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<float> BasicAttackDisjointProgression{ 306.25, 0, 306.25, 0, 306.25, 0 };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<float> RangedBasicAttackProjectileSizeProgression{ 3, 3, 3 };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<float> RangedBasicAttackProjectileSpeedProgression{ 110, 110, 110 };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<bool> bCleaveProgression{ false, false, false };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<float> CleaveDamageProgression{ 1, 1, 1 };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<float> RangedCleaveRangeProgression{ 7.5, 7.5, 7.5 };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<bool> bIsBasicAttackRangedProgression{ true, true, true };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<bool> bHasScalingPrefireProgression{ true, true, true };

	int CurrentProgression{ 0 };

	float BasicAttackStrafePenalty{ 0.8 };

	float BasicAttackBackpedalPenalty{ 0.6 };

	float BasicAttackRangedPenalty{ 0.5 };

	float BasicAttackMeleePenalty{ 0.65 };

	APlayerController* PlayerController;
	UCharacterMovementComponent* CharacterMovementComponent;

	FColor ConsoleColor{ FColor::Green };
	
	const float MovementSpeedDiminishments[2]{ 457, 540.5 };

	const float MovementSpeedDiminishmentMultipliers[2]{ 0.8, 0.5 };

	const float MinimumMovementSpeed{ 150 };

	const float MaximumDiminishedMovementSpeed{ 753.55 };

	const float ProgressionResetTime{ 1 };

	const float MaxProtection{ 325 };

	const float MaxPenetration{ 50 };

	FTimerHandle BasicAttackTimerHandle;

	FTimerDelegate BasicAttackTimerDelegate;

	FTimerHandle RangedPrefireTimerHandle;

	FTimerDelegate RangedPrefireTimerDelegate;

	FTimerHandle MeleePrefireTimerHandle;

	FTimerDelegate MeleePrefireTimerDelegate;

	FTimerHandle JumpTimerHandle;

	FTimerDelegate JumpTimerDelegate;

	FTimerHandle ProgressionResetTimerHandle;

	FTimerDelegate ProgressionResetTimerDelegate;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
