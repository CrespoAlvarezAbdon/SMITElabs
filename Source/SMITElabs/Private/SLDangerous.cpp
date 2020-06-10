// Fill out your copyright notice in the Description page of Project Settings.


#include "SLDangerous.h"

// Add default functionality here for any ISLDangerous functions that are not pure virtual.

void ISLDangerous::SetBasicAttackSpeed(float Val) { BasicAttackSpeed = Val; }

float ISLDangerous::GetCurrentBasicAttackDamage() const { return CurrentBasicAttackDamage; }

float ISLDangerous::GetPhysicalPower() const { return PhysicalPower; }

float ISLDangerous::GetMagicalPower() const { return MagicalPower; }

float ISLDangerous::GetFlatPhysicalPenetration() const { return FlatPhysicalPenetration; }

float ISLDangerous::GetFlatMagicalPenetration() const { return FlatMagicalPenetration; }

float ISLDangerous::GetPercentagePhysicalPenetration() const { return PercentagePhysicalPenetration; }

float ISLDangerous::GetPercentageMagicalPenetration() const { return PercentageMagicalPenetration; }

float ISLDangerous::GetBasicAttackPowerScaling() const { return BasicAttackPowerScaling; }

bool ISLDangerous::GetIsPhysicalDamage() const { return bIsPhysicalDamage; }