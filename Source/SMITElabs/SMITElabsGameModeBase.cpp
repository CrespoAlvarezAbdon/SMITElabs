// Copyright Epic Games, Inc. All Rights Reserved.


#include "SMITElabsGameModeBase.h"

void ASMITElabsGameModeBase::SetMovementSpeed(float Val)
{
	Cast<ASLGod>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->SetMovementSpeed(Val);
}

void ASMITElabsGameModeBase::SetBasicAttackSpeed(float Val)
{
	Cast<ASLGod>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->SetBasicAttackSpeed(Val);
}

void ASMITElabsGameModeBase::SetGodLevel(float Val)
{
	Cast<ASLGod>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->SetGodLevel(Val);
}
