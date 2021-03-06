// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XLAbilityEffect.h"
#include "XLPhaseEffect.generated.h"

/**
 * 
 */
UCLASS()
class UXLPhaseEffect : public UXLAbilityEffect
{
	GENERATED_BODY()
	
public:
	FCollisionResponseContainer Responses;

	UXLPhaseEffect();

	void Activate(class AXLCharacter* Instigator, class AXLCharacter* Target, float Strength, float Duration = 0.0f) override;
	
	void Deactivate(class AXLCharacter* Instigator, class AXLCharacter* Target, float Strength, float Duration = 0.0f) override;

};
