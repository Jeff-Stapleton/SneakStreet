#pragma once

#include "Components/ActorComponent.h"
#include "XLItemSoundManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UXLItemSoundManager : public UActorComponent
{
	GENERATED_BODY()

public:

	/** equip animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* LootCue;

	/** holster animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* EquipCue;

	/** melee animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* UnequipCue;

public:
	UXLItemSoundManager();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};