
// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtendedLibraryPCH.h"
#include "XLItem.h"

AXLItem::AXLItem()
{
	//WeaponState = CreateDefaultSubobject<USSWeaponState>(TEXT("WeaponState"));

	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh3P"));
	Mesh3P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh3P->bReceivesDecals = false;
	Mesh3P->CastShadow = true;
	Mesh3P->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh3P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Mesh3P->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
	Mesh3P->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	//Mesh3P->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	if (Character)
	{
		//AttackSpeed = Character->CharacterAttributes->GetAttackSpeed();
	}
}


void AXLItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (AttackSpeed >= 0)
	//{
	//	AttackSpeed -= DeltaTime;
	//}
}

void AXLItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DetachMeshFromPawn();
}

void AXLItem::Destroyed()
{
	Super::Destroyed();
}

UAudioComponent* AXLItem::PlaySound(USoundCue* Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound && Character)
	{
		AC = UGameplayStatics::SpawnSoundAttached(Sound, Character->GetRootComponent());
	}
	return AC;
}

float AXLItem::PlayAnimation(class UAnimMontage* Animation, float InPlayRate)
{
	float Duration = 0.0f;
	if (Character)
	{
		UAnimMontage* UseAnim = Animation;
		if (UseAnim)
		{
			Duration = Character->PlayAnimMontage(UseAnim, InPlayRate);
		}
	}
	return Duration;
}
void AXLItem::StopAnimation(class UAnimMontage* Animation)
{
	if (Character)
	{
		UAnimMontage* UseAnim = Animation;
		if (UseAnim)
		{
			Character->StopAnimMontage(UseAnim);
		}
	}
}

void AXLItem::PlayFX()
{
	/*if (WeaponEffects->GetMuzzleFX())
	{
		USkeletalMeshComponent* UseWeaponMesh = GetWeaponMesh();
		if (!WeaponEffects->IsLoopedMuzzleFX() || WeaponEffects->GetMuzzlePSC() == NULL)
		{
			if (Character != NULL)
			{
				AController* PlayerCon = Character->GetController();
				if (PlayerCon != NULL)
				{
					Mesh3P->GetSocketLocation(WeaponEffects->GetWeaponFXPoint());
					WeaponEffects->SetMuzzlePSC(UGameplayStatics::SpawnEmitterAttached(WeaponEffects->GetMuzzleFX(), Mesh3P, WeaponEffects->GetWeaponFXPoint()));
					WeaponEffects->GetMuzzlePSC()->bOwnerNoSee = false;
					WeaponEffects->GetMuzzlePSC()->bOnlyOwnerSee = false;
				}
			}
			else
			{
				WeaponEffects->SetMuzzlePSC(UGameplayStatics::SpawnEmitterAttached(WeaponEffects->GetMuzzleFX(), UseWeaponMesh, WeaponEffects->GetWeaponFXPoint()));
			}
		}
	}

	if (!WeaponEffects->IsLoopedFireAnim() || !WeaponEffects->IsPlayingFireAnim())
	{
		PlayWeaponAnimation(WeaponEffects->GetFireAnim());
		WeaponEffects->SetPlayingFireAnim(true);
	}

	if (WeaponEffects->GetFireLoopSound())
	{
		if (WeaponEffects->GetFireAC() == NULL)
		{
			WeaponEffects->SetFireAC(PlayWeaponSound(WeaponEffects->GetFireLoopSound()));
		}
	}
	else
	{
		PlayWeaponSound(WeaponEffects->GetFireStartSound());
	}*/
}
void AXLItem::StopFX()
{
	/*if (WeaponEffects->IsLoopedMuzzleFX())
	{
		if (WeaponEffects->GetMuzzlePSC() != NULL)
		{
			WeaponEffects->GetMuzzlePSC()->DeactivateSystem();
			WeaponEffects->SetMuzzlePSC(NULL);
		}
	}

	if (WeaponEffects->IsLoopedFireAnim() && WeaponEffects->IsPlayingFireAnim())
	{
		StopWeaponAnimation(WeaponEffects->GetFireAnim());
		WeaponEffects->SetPlayingFireAnim(false);
	}

	if (WeaponEffects->GetFireAC())
	{
		WeaponEffects->GetFireAC()->FadeOut(0.1f, 0.0f);
		WeaponEffects->SetFireAC(NULL);

		PlayWeaponSound(WeaponEffects->GetFireFinishSound());
	}*/
}

void AXLItem::AttachMeshToPawn()
{
	if (Character)
	{
		DetachMeshFromPawn();

		USkeletalMeshComponent* PawnMesh3p = Character->GetMesh();
		Mesh3P->SetHiddenInGame(false);
		Mesh3P->AttachToComponent(PawnMesh3p, FAttachmentTransformRules::KeepWorldTransform, AttachPoint);
	}
}
void AXLItem::DetachMeshFromPawn()
{
	Mesh3P->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh3P->SetHiddenInGame(true);
}

void AXLItem::StartEquip(class AXLCharacter* character, FName attachPoint)
{
	this->Character = character;
	this->AttachPoint = attachPoint;
	AttachMeshToPawn();

	float Duration = 0.0f;// PlayWeaponAnimation(EquipAnim);
	if (Duration <= 0.0f)
	{
		// failsafe
		Duration = 0.5f;
	}
	GetWorldTimerManager().SetTimer(TimerHandle_StopEquip, this, &AXLItem::StopEquip, Duration, false);

	if (Character && Character->IsLocallyControlled())
	{
		//PlayWeaponSound(EquipSound);
	}
}
void AXLItem::StopEquip()
{
	AttachMeshToPawn();
}

void AXLItem::Unequip()
{
	DetachMeshFromPawn();
}