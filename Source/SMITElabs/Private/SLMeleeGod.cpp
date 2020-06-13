// Fill out your copyright notice in the Description page of Project Settings.


#include "SLMeleeGod.h"

ASLMeleeGod::ASLMeleeGod()
{
	bIsBasicAttackRangedProgression = { false, false, false };
	BasicAttackDisjointProgression = { 0, 0, 0, 0, 0, 0 };
	BasicAttackRangeProgression = { 12, 12, 18 };
	BaseMovementSpeed = 594;
	BaseBasicAttackDamage = 400;
	CurrentBasicAttackDamage = BaseBasicAttackDamage;
	BasicAttackSpeed = 1;
	CurrentHealth = 2500;
}