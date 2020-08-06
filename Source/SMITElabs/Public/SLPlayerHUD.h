// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SMITELABS_API USLPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnAbilitySlotLevelled(int Index);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAbilityAimed(float ManaCost);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndAbilityAiming();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGodLevelSetByForce();
};
