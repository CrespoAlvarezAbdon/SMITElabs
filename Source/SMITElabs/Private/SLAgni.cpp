// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAgni.h"
#include "SMITElabs/Public/SLAgniFlameWave.h"

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
	BasicAttackDamageProgression = { 1 };
	BasicAttackDisjointProgression = { 0, 0 };
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

	//Essential for proper implementation of editing ability stuff in blueprints and at runtime
	SetAbilityArrays();
	SetAbilityTargeterArrays();

	FlameWaveDelayTimerDelegate.BindUFunction(this, FName("UseFlameWave"), false);
}

void ASLAgni::UseFlameWave()
{
	ASLAgniFlameWave* FlameWave = GetWorld()->SpawnActorDeferred<ASLAgniFlameWave>(FlameWaveProjectile, GetTransform());
	FlameWave->SetOrigin(this);
	FlameWave->SetDamage(FlameWaveDamage[AbilitySlotPoints[0]]);
	FlameWave->SetScaling(FlameWaveScaling);
	UGameplayStatics::FinishSpawningActor(FlameWave, FlameWave->GetTransform());
}

void ASLAgni::OnBasicAttackHit(TArray<ISLVulnerable*> Targets)
{
	Super::OnBasicAttackHit(Targets);

	for (ISLVulnerable* CurrentTarget : Targets)
	{
		if (CombustionCount < 4)
		{ 
			++CombustionCount;
			if (CombustionCount >= 4)GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Combustion is ready!"));
			else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Combustion Passive Count: %i"), CombustionCount));
		}
	}
}

void ASLAgni::FireAbility(int AbilitySlot)
{
	if (AbilitySlotAbilities[AbilitySlot] == PrimedAbility)
	{
		if ((int)CurrentMana >= (int)CurrentAbilityManaCosts[AbilitySlot])
		{
			switch (AbilitySlot)
			{
			case 0:
				break;
			case 1:
				GetWorld()->GetTimerManager().SetTimer(FlameWaveDelayTimerHandle, FlameWaveDelayTimerDelegate, .5, false);
				Super::FireAbility(AbilitySlot);
				break;
			case 2:
				break;
			case 3:
				break;
			}
		}
	}
}

void ASLAgni::ConsumeCombustionStacks()
{
	CombustionCount = 0;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Combustion Stacks Consumed."));
}
