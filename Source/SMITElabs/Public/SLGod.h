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

	void LookUp(float Val);

	void TurnRight(float Val);

	void MoveForward(float Val);

	void MoveRight(float Val);

	void MoveDiagonally(int ValX, int ValY);

	void SetMovementSpeed(float Val);

	void SetBasicAttackSpeed(float Val);

	virtual void SetCurrentHealth(float Val, AActor* Origin) override;

	float GetCurrentBasicAttackDamage();

	float GetPhysicalPower();

	float GetMagicalPower();

	float GetBasicAttackPowerScaling();

	bool GetIsPhysicalDamage();

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

	float BaseMovementSpeed{ 360 };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float UndiminishedMovementSpeed{ BaseMovementSpeed };

	float DiminishedMovementSpeed{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float BasicAttackSpeed{ 1 };

	bool bIsBasicAttacking{ false };

	bool bIsJumping{ false };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	bool bFatalis{ false };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	bool bHasMinPrefire{ true };

	float BasicAttackPenalty{ 1 };

	float BaseBasicAttackDamage{ 43 };

	float LevelBasicAttackDamage{ 1 };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float CurrentBasicAttackDamage{ BaseBasicAttackDamage };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float BasicAttackPowerScaling{ 1 };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float RangedBasicProjectileSpeed{ 110 };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float PhysicalPower{ 0 };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float MagicalPower{ 0 };

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
	TArray<float> RangedBasicAttackProjectileSizeProgression{ 3, 3, 3 };

	UPROPERTY(EditAnywhere, Category = "Statistics")
	TArray<float>  BasicAttackDisjointProgression{ 306.25, 0, 306.25, 0, 306.25, 0 };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<bool> bCleaveProgression{ false, false, false };

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<bool> bIsBasicAttackRangedProgression{ true, true, true };

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
