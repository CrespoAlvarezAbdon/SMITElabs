// Fill out your copyright notice in the Description page of Project Settings.


#include "SLVulnerable.h"

// Add default functionality here for any ISLDamageable functions that are not pure virtual.

void ISLVulnerable::TakeHealthDamage(float Val, ISLDangerous* Origin)
{
	CurrentHealth -= Val;
}

float ISLVulnerable::GetPhysicalProtections() { return PhysicalProtections; }

float ISLVulnerable::GetMagicalProtections() { return MagicalProtections; }
