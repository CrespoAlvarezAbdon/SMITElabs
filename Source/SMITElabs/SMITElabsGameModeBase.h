// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SMITElabs/Public/SLGod.h"
#include "SMITElabsGameModeBase.generated.h"

class UGameplayStatics;
class ASLGod;

/**
 * 
 */
UCLASS()
class SMITELABS_API ASMITElabsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec, Category = "ExecFunctions")
	void SetMovementSpeed(float val);

	UFUNCTION(Exec, Category = "ExecFunctions")
	void SetBasicAttackSpeed(float val);
};
