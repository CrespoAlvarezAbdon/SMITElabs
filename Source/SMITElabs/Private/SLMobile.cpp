// Fill out your copyright notice in the Description page of Project Settings.


#include "SLMobile.h"

// Add default functionality here for any ISLMobile functions that are not pure virtual.

void ISLMobile::SetMovementSpeed(float Val)
{
	UndiminishedMovementSpeed = Val;
	if (UndiminishedMovementSpeed >= MovementSpeedDiminishments[0])
	{
		if (UndiminishedMovementSpeed >= MovementSpeedDiminishments[1])
		{
			DiminishedMovementSpeed = MovementSpeedDiminishments[0] + (MovementSpeedDiminishments[1] - MovementSpeedDiminishments[0]) * MovementSpeedDiminishmentMultipliers[0] + (Val - MovementSpeedDiminishments[1]) * MovementSpeedDiminishmentMultipliers[1];
		}
		else
		{
			DiminishedMovementSpeed = MovementSpeedDiminishments[0] + (Val - MovementSpeedDiminishments[0]) * MovementSpeedDiminishmentMultipliers[0];
		}
	}
	else if (UndiminishedMovementSpeed < MinimumMovementSpeed)
	{
		UndiminishedMovementSpeed = MinimumMovementSpeed;
		DiminishedMovementSpeed = MinimumMovementSpeed;
	}
	else
	{
		DiminishedMovementSpeed = UndiminishedMovementSpeed;
	}
}