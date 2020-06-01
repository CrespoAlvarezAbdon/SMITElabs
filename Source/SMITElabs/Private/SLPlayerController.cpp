// Fill out your copyright notice in the Description page of Project Settings.


#include "SLPLayerController.h"

void ASLPlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	UGameplayStatics::GetPlayerCameraManager(pawn, 0)->ViewPitchMin = 290;
	UGameplayStatics::GetPlayerCameraManager(pawn, 0)->ViewPitchMax = 55;
}