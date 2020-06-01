// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "SLPlayerController.generated.h"

class UGameplayStatics;
class APlayerCameraManager;

UCLASS()
class SMITELABS_API ASLPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* pawn) override;
};
