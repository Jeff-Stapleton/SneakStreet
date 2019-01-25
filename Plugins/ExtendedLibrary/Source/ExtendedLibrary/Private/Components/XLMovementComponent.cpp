// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtendedLibraryPCH.h"
#include "XLMovementComponent.h"

UXLMovementComponent::UXLMovementComponent()
{
	JumpZVelocity = JumpVelocity * JumpVelocityModifier;
	MaxWalkSpeed = BaseMovementSpeed * MovementSpeedModifier;
}

void UXLMovementComponent::UpdateMovementSpeed()
{
	AXLCharacter* Pawn = Cast<AXLCharacter>(GetOwner());
	if (Pawn->ActionState == EActionState::Sprinting)
	{
		MaxWalkSpeed = RunningMovementSpeed * MovementSpeedModifier;
	}
	else if (Pawn->PostureState == EPostureState::Crouching)
	{
		MaxWalkSpeed = CrouchedMovementSpeed * MovementSpeedModifier;
	}
	else if (Pawn->PostureState == EPostureState::Prone)
	{
		MaxWalkSpeed = PronedMovementSpeed * MovementSpeedModifier;
	}
	else
	{
		MaxWalkSpeed = BaseMovementSpeed * MovementSpeedModifier;
	}
}

void UXLMovementComponent::ModifyJumpHeight(float NewModifier)
{
	JumpVelocityModifier = NewModifier;
	JumpZVelocity = JumpVelocity * JumpVelocityModifier;
}