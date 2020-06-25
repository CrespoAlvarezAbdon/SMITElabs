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
#include "SMITElabs/Public/SLVulnerable.h"
#include "SMITElabs/Public/SLDangerous.h"
#include "SMITElabs/Public/SLMobile.h"
#include "SMITElabs/Public/SLIdentifiable.h"
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
class ISLVulnerable;
class ISLDangerous;
class ISLMobile;
class ISLIdentifiable;
struct FTimerHandle;

/**
 *
 */
UCLASS(Abstract)
class SMITELABS_API ASLGod : public ACharacter, public ISLVulnerable, public ISLDangerous, public ISLMobile, public ISLIdentifiable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASLGod();

	virtual bool GetBIsOrder() const override;

	virtual float GetPhysicalProtections() const override;

	virtual float GetMagicalProtections() const override;

	virtual void SetMovementSpeed(float Val) override;

	virtual void SetBasicAttackSpeed(float Val) override;

	void SetGodLevel(int Val);

	virtual float GetCurrentBasicAttackDamage() const override;

	virtual float GetPhysicalPower() const override;

	virtual float GetMagicalPower() const override;

	virtual float GetFlatPhysicalPenetration() const override;

	virtual float GetFlatMagicalPenetration() const override;

	virtual float GetPercentagePhysicalPenetration() const override;

	virtual float GetPercentageMagicalPenetration() const override;

	virtual float GetBasicAttackPowerScaling() const override;

	virtual bool GetIsPhysicalDamage() const override;

	virtual float CalculateTotalProtections(ISLVulnerable* Targeted) const override;

	void LookUp(float Val);

	void TurnRight(float Val);

	void MoveForward(float Val);

	void MoveRight(float Val);

	void MoveDiagonally(int ValX, int ValY);

	virtual void UseAbility1();

	virtual void UseAbility2();

	virtual void UseAbility3();

	virtual void UseAbility4();

	void AimAbility1();

	void AimAbility2();

	void AimAbility3();

	virtual void AimAbility4();

	void LevelAbility1();

	void LevelAbility2();

	void LevelAbility3();

	virtual void LevelAbility4();

	void OnBeginJump();

	UFUNCTION()
	void OnEndJump();

	void OnBeginFireBasicAttack();

	void OnEndFireBasicAttack();

	void BeginFireBasicAttack();

	UFUNCTION()
	void NextBasicAttack();

	UFUNCTION()
	void OnPrefireTimerEnd();
	
	void FireRangedBasicAttack();

	UFUNCTION()
	void FireMeleeBasicAttack();

	UFUNCTION()
	void ResetProgression();

	UFUNCTION()
	void FinishResetProgression();

	void ChangeBasicAttackTargeter();

	UFUNCTION()
	void SustainPerFive();

	void ActivateCooldownTimer(FTimerHandle& CooldownTimer, float CooldownDuration, FString AbilityName, float AbilityManaCost, bool bUsesCDR);

	bool IsAbilityAvailable(FTimerHandle& CooldownTimer, int AbilityLevel, TArray<float> AbilityManaCost, FString AbilityName, bool bAbilityIsPrimed);

	void CancelAbility();

	virtual void OnBasicAttackHit(TArray<ISLVulnerable*> Targets) override;

	virtual void TakeHealthDamage(float Val, ISLDangerous* Origin) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetBaseStatistics();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RangeLineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RangedAimComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeleeAimComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* AbilityAimComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MTargeterReset;

	UStaticMeshComponent* CurrentAimComponent;

	APlayerController* PlayerController;
	UCharacterMovementComponent* CharacterMovementComponent;

	FColor ConsoleColor{ FColor::Green };

	FTimerHandle Ability1CooldownTimerHandle;

	FTimerHandle Ability2CooldownTimerHandle;

	FTimerHandle Ability3CooldownTimerHandle;

	FTimerHandle Ability4CooldownTimerHandle;

	FTimerHandle PerFiveTimerHandle;

	FTimerDelegate PerFiveTimerDelegate;

	FTimerHandle BasicAttackTimerHandle;

	FTimerDelegate BasicAttackTimerDelegate;

	FTimerHandle PrefireTimerHandle;

	FTimerDelegate PrefireTimerDelegate;

	FTimerHandle ProgressionResetTimerHandle;

	FTimerDelegate ProgressionResetTimerDelegate;

	FTimerHandle FinishProgressionResetTimerHandle;

	FTimerDelegate FinishProgressionResetTimerDelegate;

	FTimerHandle JumpTimerHandle;

	FTimerDelegate JumpTimerDelegate;

#pragma region Offense

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	float BaseBasicAttackSpeed{ 1 };

	float CurrentBasicAttackSpeed{ 0 };

	// Used as a percentage increase of the base rather than a flat numerical
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	float BasicAttackSpeedPerLevel{ .012 };

	bool bIsBasicAttacking{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	bool bFatalis{ false };

	float BasicAttackPenalty{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	float BaseBasicAttackDamage{ 43 };

	float CurrentBasicAttackDamage{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	float BasicAttackDamagePerLevel{ 1.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	float BasicAttackPowerScaling{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	float BasicAttackRangedPenalty{ 0.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	float BasicAttackMeleePenalty{ 0.65 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	float PhysicalPower{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	float MagicalPower{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Power")
	bool bIsPhysicalDamage{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	float FlatPhysicalPenetration{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	float FlatMagicalPenetration{ 0 };

	const float MaxFlatPenetration{ 50 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	float PercentagePhysicalPenetration{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	float PercentageMagicalPenetration{ 0 };

	const float MaxPercentagePenetration{ .4 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<float> BasicAttackRefireProgression{ 0.5, 1, 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<float> BasicAttackPrefireProgression{ 0.25, 0.25, 0.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<float> BasicAttackRangeProgression{ 55, 55, 55 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<float> BasicAttackDamageProgression{ BasicAttackRefireProgression };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<float> BasicAttackDisjointProgression{ 0, 0, 0, 0, 0, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<float> RangedBasicAttackProjectileSizeProgression{ 3, 3, 3 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<float> RangedBasicAttackProjectileSpeedProgression{ 110, 110, 110 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<bool> bCleaveProgression{ false, false, false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<float> CleaveDamageProgression{ 1, 1, 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<float> RangedCleaveRangeProgression{ 7.5, 7.5, 7.5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<bool> bIsBasicAttackRangedProgression{ true, true, true };

	// If disabled, Prefire shouldn't exceed (Refire / 2.5), otherwise attack speed will become inconsistent because the Postfire will be less than 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<bool> bHasScalingPrefireProgression{ true, true, true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	float ProgressionResetTime{ 1 };

	int CurrentProgression{ 0 };

	bool bInitialProgressionReset{ true };

#pragma endregion

#pragma region Defense

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float BasePhysicalProtections{ 11 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float BaseMagicalProtections{ 30 };

	float CurrentPhysicalProtections{ 0 };

	float CurrentMagicalProtections{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float PhysicalProtectionsPerLevel{ 2.6 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float MagicalProtectionsPerLevel{ .9 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float BaseHealth{ 600 };

	float CurrentHealth{ 0 };

	float MaxHealth{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float HealthPerLevel{ 71 };

	float Shield{ 0 };

	const float MaxProtection{ 325 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool bHasBasicHealthBar{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	int NumberOfBasics{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float BaseHealthPerFive{ 7 };

	float CurrentHealthPerFive{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float HealthPerFivePerLevel{ .47 };

	const float MaxPerFive{ 100 };

#pragma endregion

#pragma region Movement

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseMovementSpeed{ 360 };

	float UndiminishedMovementSpeed{ 0 };

	float DiminishedMovementSpeed{ 0 };

	const float MovementSpeedDiminishments[2]{ 457, 540.5 };

	const float MovementSpeedDiminishmentMultipliers[2]{ 0.8, 0.5 };

	const float MinimumMovementSpeed{ 150 };

	const float MaximumDiminishedMovementSpeed{ 753.55 };

	float StrafePenalty{ 0.8 };

	float BackpedalPenalty{ 0.6 };

	bool bIsJumping{ false };

#pragma endregion

#pragma region Identity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FString UnitName{ "Unknown" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	bool bIsOrder{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	bool bIsNeutral{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	int GodLevel{ 20 };

#pragma endregion

#pragma region Ability

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float CooldownReductionPercentage{ 0 };

	int AbilityPoints{ 0 };

	int Ability1Level{ 0 };
	int Ability2Level{ 0 };
	int Ability3Level{ 0 };
	int Ability4Level{ 0 };

	bool bAbility1IsPrimed{ false };
	bool bAbility2IsPrimed{ false };
	bool bAbility3IsPrimed{ false };
	bool bAbility4IsPrimed{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float BaseMana{ 255 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float CurrentMana{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float MaxMana{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float ManaPerLevel{ 45 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float BaseManaPerFive{ 4.7 };

	float CurrentManaPerFive{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float ManaPerFivePerLevel{ .37 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category = "Ability")
	TArray<float> Ability1Cooldowns = { 12, 12, 12, 12, 12 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category = "Ability")
	TArray<float>  Ability2Cooldowns = { 15, 14, 13, 12, 11 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category = "Ability")
	TArray<float>  Ability3Cooldowns = { 15, 15, 15, 15, 15 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category = "Ability")
	TArray<float>  Ability4Cooldowns = { 18, 18, 18, 18, 18 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category = "Ability")
	TArray<float>  Ability1ManaCost = { 60, 65, 70, 75, 80 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category = "Ability")
	TArray<float>  Ability2ManaCost = { 60, 70, 80, 90, 100 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category = "Ability")
	TArray<float>  Ability3ManaCost = { 70, 75, 80, 85, 90 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category = "Ability")
	TArray<float>  Ability4ManaCost = { 0, 0, 0, 0, 0 };
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> Ability1TargeterComponents;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> Ability2TargeterComponents;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> Ability3TargeterComponents;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> Ability4TargeterComponents;

#pragma endregion

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
