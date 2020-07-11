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

	FlameWavePrefireTimerDelegate.BindUFunction(this, FName("UseFlameWave"), false);
}

TArray<ASLGod*> ASLAgni::GetCombustionTargets()
{
	return CombustionTargets;
}

void ASLAgni::AddCombustionTarget(ASLGod* CombustionTarget)
{
	CombustionTargets.Push(CombustionTarget);
	CombustionTimerHandles.Push(FTimerHandle());
	CombustionTicks.Push(6);
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("DealCombustionDamage"), CombustionTargets.Last(), CombustionTimerHandles.Last(), CombustionTicks.Last());
	GetWorldTimerManager().SetTimer(CombustionTimerHandles.Last(), TimerDelegate, .5, false);
}

void ASLAgni::UseFlameWave()
{
	ASLAgniFlameWave* FlameWave = GetWorld()->SpawnActorDeferred<ASLAgniFlameWave>(FlameWaveProjectile, GetTransform());
	FlameWave->SetOrigin(this);
	FlameWave->SetDamage(FlameWaveDamage[AbilitySlotPoints[1] - 1]);
	FlameWave->SetScaling(FlameWaveScaling);
	if (CombustionCount >= 4)
	{ 
		ConsumeCombustionStacks();
		FlameWave->SetBHasCombustion(true);
	}
	else FlameWave->SetBHasCombustion(false);
	UGameplayStatics::FinishSpawningActor(FlameWave, FlameWave->GetTransform());
	PrimedAbility = -1;
	bCanFireAbility = true;
	BeginFireBasicAttack();
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
	if (AbilitySlotAbilities[AbilitySlot] == PrimedAbility && (int)CurrentMana >= (int)CurrentAbilityManaCosts[AbilitySlot] && bCanFireAbility)
	{
		switch (AbilitySlot)
		{
		case 0:
			Super::FireAbility(AbilitySlot);
			break;
		case 1:
			GetWorld()->GetTimerManager().SetTimer(FlameWavePrefireTimerHandle, FlameWavePrefireTimerDelegate, .5, false);
			Super::FireAbility(AbilitySlot);
			bCanFireAbility = false;
			break;
		case 2:
			Super::FireAbility(AbilitySlot);
			break;
		case 3:
			Super::FireAbility(AbilitySlot);
			break;
		}
	}
}

void ASLAgni::ConsumeCombustionStacks()
{
	CombustionCount = 0;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Combustion Stacks Consumed."));
}

void ASLAgni::DealCombustionDamage(ASLGod* CombustionTarget, FTimerHandle CombustionTimerHandle, int& CombustionTick)
{
	CombustionTarget->TakeHealthDamage((CombustionDamage + GetMagicalPower() * CombustionScaling) * (100 / (CalculateTotalProtections(CombustionTarget) + 100)), this);
	FTimerDelegate TimerDelegate;
	--CombustionTick;
	if(CombustionTick > 0)
	{
		TimerDelegate.BindUFunction(this, FName("DealCombustionDamage"), CombustionTarget, CombustionTimerHandle, CombustionTick);
		GetWorldTimerManager().SetTimer(CombustionTimerHandle, TimerDelegate, .5, false);
	}
	else
	{
		CombustionTargets.RemoveAt(0);
		CombustionTimerHandles.RemoveAt(0);
		CombustionTicks.RemoveAt(0);
	}
}
