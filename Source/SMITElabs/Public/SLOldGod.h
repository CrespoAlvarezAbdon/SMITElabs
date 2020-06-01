/// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SLOldGod.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UGameplayStatics;
class APlayerController;
class UCharacterMovementComponent;
class FTimerManager;
struct FTimerHandle;

UCLASS()
class SMITELABS_API ASLOldGod : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASLOldGod();

	void LookUp(float val);

	void TurnRight(float val);

	void MoveForward(float val);

	void MoveRight(float val);

	void MoveDiagonally(int valX, int valY);

	UFUNCTION()
	void SetIsBasicAttacking(bool val);

	void SetMovementSpeed(float val);

	void SetBasicAttackSpeed(float val);

	void OnJump();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Statistics")
	float UndiminishedMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Statistics")
	float DiminishedMovementSpeed;

	APlayerController* PlayerController;

	float BasicAttackSpeed;

	FColor consoleColor;

	bool bIsBasicAttacking;

	bool bIsRangedBasicAttack;

	FTimerHandle BasicAttackTimerHandle;

	FTimerDelegate BasicAttackTimerDelegate;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
