// Copyright Epic Games, Inc. All Rights Reserved.


#include "SMITElabsGameModeBase.h"

void ASMITElabsGameModeBase::SetMovementSpeed(float val)
{
	Cast<ASLGod>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->SetMovementSpeed(val);
}

void ASMITElabsGameModeBase::SetBasicAttackSpeed(float val)
{
	Cast<ASLGod>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->SetBasicAttackSpeed(val);
}
