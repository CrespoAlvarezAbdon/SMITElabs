// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLPlayerHUDWorld.generated.h"

class ASLGod;

/**
 * 
 */
UCLASS()
class SMITELABS_API USLPlayerHUDWorld : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void GetWidgetComponentOwner(ASLGod* God);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMaxHealthChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMaxManaChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGodLevelChanged();
};
