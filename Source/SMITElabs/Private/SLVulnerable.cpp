// Fill out your copyright notice in the Description page of Project Settings.


#include "SLVulnerable.h"

// Add default functionality here for any ISLDamageable functions that are not pure virtual.

float ISLVulnerable::GetPhysicalProtections() { return PhysicalProtections; }

float ISLVulnerable::GetMagicalProtections() { return MagicalProtections; }
