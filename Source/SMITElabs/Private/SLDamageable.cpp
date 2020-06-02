// Fill out your copyright notice in the Description page of Project Settings.


#include "SLDamageable.h"

// Add default functionality here for any ISLDamageable functions that are not pure virtual.

void ISLDamageable::SetCurrentHealth(float Val, AActor* Origin) { CurrentHealth -= Val; }

float ISLDamageable::GetPhysicalProtections() { return PhysicalProtections; }

float ISLDamageable::GetMagicalProtections() { return MagicalProtections; }
