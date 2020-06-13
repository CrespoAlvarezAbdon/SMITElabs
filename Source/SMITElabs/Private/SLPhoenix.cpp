// Fill out your copyright notice in the Description page of Project Settings.


#include "SLPhoenix.h"

ASLPhoenix::ASLPhoenix()
{
	BaseHealth = 3000;
	CurrentHealth = BaseHealth;
	TowerMaxRegenHealth = BaseHealth * .7;
	CurrentBasicAttackDamage = 300;
	PhysicalProtections = 125;
	MagicalProtections = 125;
	bHasShaft = false;
	bHasTowerHPS = true;
	if (!bHasShaft) { TowerShaftComponent->ToggleVisibility(false); TowerHeadComponent->SetRelativeLocation(FVector(TowerHeadComponent->GetRelativeLocation().X, TowerHeadComponent->GetRelativeLocation().Y, TowerHeadComponent->GetRelativeLocation().Z - 1250)); }
}
