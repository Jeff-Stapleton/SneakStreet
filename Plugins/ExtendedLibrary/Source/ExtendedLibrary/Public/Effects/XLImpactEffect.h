#include "GameFramework/Actor.h"
#include "Structs/XLDecalData.h"
#include "XLImpactEffect.generated.h"

#pragma once

UCLASS(Abstract, Blueprintable)
class AXLImpactEffect : public AActor
{
	GENERATED_BODY()

public:
	AXLImpactEffect();

	/** explosion light */
	UPROPERTY(VisibleDefaultsOnly, Category = Effect)
	UPointLightComponent* ImpactLight;

	/** how long keep explosion light on? */
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	float ImpactLightFadeOut;

	/** default impact FX used when material specific override doesn't exist */
	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	UParticleSystem* DefaultFX;

	/** impact FX on concrete */
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	UParticleSystem* ConcreteFX;

	/** impact FX on dirt */
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	UParticleSystem* DirtFX;

	/** impact FX on water */
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	UParticleSystem* WaterFX;

	/** impact FX on metal */
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	UParticleSystem* MetalFX;

	/** impact FX on wood */
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	UParticleSystem* WoodFX;

	/** impact FX on glass */
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	UParticleSystem* GlassFX;

	/** impact FX on grass */
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	UParticleSystem* GrassFX;

	/** impact FX on flesh */
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	UParticleSystem* FleshFX;

	UPROPERTY(EditDefaultsOnly, Category = Visual)
	UParticleSystem* EnergyFX;

	/** default impact sound used when material specific override doesn't exist */
	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	USoundBase* DefaultSound;

	/** impact FX on concrete */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* ConcreteSound;

	/** impact FX on dirt */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* DirtSound;

	/** impact FX on water */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* WaterSound;

	/** impact FX on metal */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* MetalSound;

	/** impact FX on wood */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* WoodSound;

	/** impact FX on glass */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* GlassSound;

	/** impact FX on grass */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* GrassSound;

	/** impact FX on flesh */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* FleshSound;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* EnergySound;

	/** default decal when material specific override doesn't exist */
	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	struct FDecalData DefaultDecal;

	/** surface data for spawning */
	UPROPERTY(BlueprintReadOnly, Category = Surface)
	FHitResult SurfaceHit;

	/** spawn explosion */
	virtual void BeginPlay() override;

	/** update fading light */
	virtual void Tick(float DeltaSeconds) override;

protected:

	/** Point light component name */ //maybe
	FName ImpactLightComponentName;

	/** get FX for material type */
	UParticleSystem* GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const;

	/** get sound for material type */
	USoundBase* GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const;
};
