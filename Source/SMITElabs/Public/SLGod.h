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
#include "SMITElabs/Public/SLPLayerHUD.h"
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
class USLPlayerHUD;
struct FTimerHandle;

DECLARE_DELEGATE_OneParam(FAbilityDelegate, int);

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

	USceneComponent* GetTargeterLocationComponent();

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

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	int GetAbilitySlotAbility(int Index);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	int GetAbilitySlotPointCount(int Index);

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

	void SetAbilityArrays();

	void SetAbilityTargeterArrays();

	virtual void LevelAbility(int AbilitySlot);

	void AimAbility(int AbilitySlot);

	void CancelAbility();

	UFUNCTION()
	virtual void OnAbilityCooldownEnded(int AbilityID);

	void StartAbilityCooldown(int AbilitySlot);

	virtual void FireAbility(int AbilitySlot);

	virtual void OnBasicAttackHit(TArray<ISLVulnerable*> Targets) override;

	virtual void TakeHealthDamage(float Val, ISLDangerous* Origin) override;

	virtual void BecomeStunned(float Duration) override;

	virtual void RemoveStun() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class USLPlayerHUD> PlayerHUDClass;

	USLPlayerHUD* PlayerHUD;

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
	USceneComponent* TargeterLocationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmNoseComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* MAbilityTargeter;

	UStaticMeshComponent* CurrentAimComponent;

	APlayerController* PlayerController;
	UCharacterMovementComponent* CharacterMovementComponent;

	FColor ConsoleColor{ FColor::Green };

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

	FTimerHandle StunTimerHandle;

#pragma region Offense

	bool bCanBasicAttack{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	float BaseBasicAttackSpeed{ 1 };

	float CurrentBasicAttackSpeed{ 0 };

	// Used as a percentage increase of the base rather than a flat numerical
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
	float BasicAttackSpeedPerLevel{ .012 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	// If disabled, Prefire shouldn't exceed (Refire / 2.5), otherwise attack speed will become inconsistent because the Postfire will be less than 0
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

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	float CurrentHealth{ 0 };

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeedPerLevel{ 0 };

	float UndiminishedMovementSpeed{ 0 };

	float DiminishedMovementSpeed{ 0 };

	const float MovementSpeedDiminishments[2]{ 457, 540.5 };

	const float MovementSpeedDiminishmentMultipliers[2]{ 0.8, 0.5 };

	const float MinimumMovementSpeed{ 150 };

	const float MaximumDiminishedMovementSpeed{ 753.55 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StrafePenalty{ 0.8 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BackpedalPenalty{ 0.6 };

	bool bIsJumping{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanMove{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanTurn{ true };

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

	int AbilitySlotPoints[4] = { 0, 0, 0, 0 };

	int AbilitySlotAbilities[4] = { 0, 1, 2, 3 };

	bool bCanFireAbility{ true };

	float BaseMaxTargeterRange{ 7000 };

	float CurrentMaxTargeterRange{ BaseMaxTargeterRange };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float BaseMana{ 255 };

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	float CurrentMana{ 0 };

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	float MaxMana{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float ManaPerLevel{ 45 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float BaseManaPerFive{ 4.7 };

	float CurrentManaPerFive{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float ManaPerFivePerLevel{ .37 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability", meta = (ClampMin = "4.0"))
	int NumberOfAbilities{ 4 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<int> AbilitySlots = { 0, 1, 2, 3 };

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<UStaticMeshComponent*> AbilityTargeterComponents;

	TArray<int> ActiveAbilityTargeterComponentIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<UStaticMesh*> AbilityTargeterMeshes;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	TArray<FTimerHandle> AbilityCooldownTimerHandles;

	TArray<FTimerDelegate> AbilityCooldownTimerDelegates;

	int PrimedAbility{ -1 };

	int AbilityKeyDown{ -1 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<FString> AbilityNames = { "Noxious Fumes", "Flame Wave", "Path of Flames", "Rain Fire" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityMaxRanges = { 55, 70, 70, 65 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<int> MaxAbilityCharges = { 1, 1, 1, 3 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<int> CurrentAbilityCharges = { 1, 1, 1, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<int> AbilityChargesAtLevelOne = { 1, 1, 1, 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankOneCooldowns = { 12, 15, 15, 18 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankTwoCooldowns = { 12, 14, 15, 18 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankThreeCooldowns = { 12, 13, 15, 18 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankFourCooldowns = { 12, 12, 15, 18 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankFiveCooldowns = { 12, 11, 15, 18 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankOneManaCosts = { 60, 60, 70, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankTwoManaCosts = { 65, 70, 75, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankThreeManaCosts = { 70, 80, 80, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankFourManaCosts = { 75, 90, 85, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityRankFiveManaCosts = { 80, 100, 90, 0 };

	TArray<float> CurrentAbilityCooldowns = { AbilityRankOneCooldowns[0], AbilityRankOneCooldowns[1], AbilityRankOneCooldowns[2], AbilityRankOneCooldowns[3] };

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	TArray<float> CurrentAbilityManaCosts = { AbilityRankOneManaCosts[0], AbilityRankOneManaCosts[1], AbilityRankOneManaCosts[2], AbilityRankOneManaCosts[3] };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityTargeterScalesX = { 40, 50, 60, 40 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityTargeterScalesY = { 40, 15, 10, 40 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityTargeterPositionsX = { 0, 0, -500, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityTargeterPositionsY = { 0, 0, 0, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<float> AbilityTargeterRotations = { 0, 0, 0, 0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<int> ATCCount = { 1, 1, 1, 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<bool> bAreInstantCast = { false, false, false, false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "Ability")
	TArray<bool> bFollowGroundTargeter = { true, false, false, true };

#pragma endregion

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
