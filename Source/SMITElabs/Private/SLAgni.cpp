// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAgni.h"

ASLAgni::ASLAgni()
{
	UnitName = "Agni";
	BaseMovementSpeed = 355;
	BaseBasicAttackDamage = 34;
	BasicAttackDamagePerLevel = 1.5;
	BasicAttackPowerScaling = 0.2;
	BaseBasicAttackSpeed = 1;
	BasicAttackSpeedPerLevel = .012;
	BasicAttackRefireProgression = { 1 };
	BasicAttackPrefireProgression = { .15 };
	BasicAttackRangeProgression = { 55 };
	BasicAttackDamageProgression = { .2 };
	BasicAttackDisjointProgression = { 156.25, 0 };
	RangedBasicAttackProjectileSizeProgression = { 3 };
	RangedBasicAttackProjectileSpeedProgression = { 100 };
	bCleaveProgression = { false };
	CleaveDamageProgression = { 0 };
	RangedCleaveRangeProgression = { 0 };
	bIsBasicAttackRangedProgression = { true };
	bHasScalingPrefireProgression = { false };
	bIsPhysicalDamage = false;
	BaseHealth = 360;
	BasePhysicalProtections = 11;
	PhysicalProtectionsPerLevel = 2.6;
	BaseMagicalProtections = 30;
	MagicalProtectionsPerLevel = .9;
	BaseHealthPerFive = 7;
	HealthPerFivePerLevel = .47;
	Ability1Cooldowns = { 12, 12, 12, 12, 12 };
	Ability2Cooldowns = { 15, 14, 13, 12, 11 };
	Ability3Cooldowns = { 15, 15, 15, 15, 15 };
	Ability4Cooldowns = { 18, 18, 18, 18, 18 };
	Ability1ManaCost = { 60, 65, 70, 75, 80 };
	Ability2ManaCost = { 60, 70, 80, 90, 100 };
	Ability3ManaCost = { 70, 75, 80, 85, 90 };
	Ability4ManaCost = { 0, 0, 0, 0, 0 };
}
