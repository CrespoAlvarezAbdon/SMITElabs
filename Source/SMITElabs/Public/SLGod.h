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

	void ChangeBasicAttackTargeter();

	virtual void TakeHealthDamage(float Val, ISLDangerous* Origin) override;

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

	UStaticMeshComponent* CurrentAimComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP Variables")
	bool bIsOrderBP{ true };

	APlayerController* PlayerController;
	UCharacterMovementComponent* CharacterMovementComponent;

	FColor ConsoleColor{ FColor::Green };

	FTimerHandle BasicAttackTimerHandle;

	FTimerDelegate BasicAttackTimerDelegate;

	FTimerHandle PrefireTimerHandle;

	FTimerDelegate PrefireTimerDelegate;

	bool bIsJumping{ false };

	FTimerHandle JumpTimerHandle;

	FTimerDelegate JumpTimerDelegate;

	FTimerHandle ProgressionResetTimerHandle;

	FTimerDelegate ProgressionResetTimerDelegate;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
