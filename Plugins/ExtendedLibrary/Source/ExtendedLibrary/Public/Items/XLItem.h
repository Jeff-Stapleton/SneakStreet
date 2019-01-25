#include "GameFramework/Actor.h"
#include "Enums/XLWeaponState.h"
#include "XLItem.generated.h"

#pragma once

UCLASS()
class AXLItem : public AActor
{
	GENERATED_BODY()
	
public:
	AXLItem();

	void PostInitializeComponents() override;

	void Destroyed() override;

	virtual void Tick(float DeltaSeconds) override;

	UAudioComponent* PlaySound(USoundCue* Sound);

	float PlayAnimation(class UAnimMontage* Animation, float InPlayRate = 1.f);
	void StopAnimation(class UAnimMontage* Animation);

	virtual void PlayFX();
	virtual void StopFX();

	void AttachMeshToPawn();
	void DetachMeshFromPawn();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void StartEquip(AXLCharacter* Character, FName AttachPoint);
	void StopEquip();

	void Unequip();

	UPROPERTY()
	AXLCharacter* Character;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh3P;

	UPROPERTY()
	FName AttachPoint;

	FTimerHandle TimerHandle_StopEquip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UXLWeaponAnimationManager* WeaponAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UXLWeaponEffectManager* WeaponEffects;

};