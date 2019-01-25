
// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtendedLibraryPCH.h"
#include "XLCharacter.h"

AXLCharacter::AXLCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UXLMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CharacterResources = CreateDefaultSubobject<UXLCharacterResources>(TEXT("CharacterResources"));
	CharacterStats = CreateDefaultSubobject<UXLCharacterStats>(TEXT("CharacterStats"));
	CharacterWeapons = CreateDefaultSubobject<UXLWeaponManager>(TEXT("CharacterWeapons"));
	CharacterAbilities = CreateDefaultSubobject<UXLAbilityManager>(TEXT("CharacterAbilities"));
	CharacterAnimations = CreateDefaultSubobject<UXLPlayerAnimationManager>(TEXT("CharacterAnimations"));
	CharacterEffects = CreateDefaultSubobject<UXLPlayerEffectManager>(TEXT("CharacterEffects"));
	CoverComponent = CreateDefaultSubobject<UXLCoverComponent>(TEXT("CoverComponent"));
	MovementComponent = Cast<UXLMovementComponent>(GetMovementComponent());

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	PrimaryActorTick.bCanEverTick = true;

	ActionState = EActionState::None;
	CombatState = ECombatState::Passive;
	HealthState = EHealthState::Alive;
	MovementState = EMovementState::Idle;
	PostureState = EPostureState::Standing;
}

void AXLCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AXLCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MovementComponent->UpdateMovementSpeed();
}


void AXLCharacter::Move(float Value)
{
	if (XLCharacterCan::Move(this))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AXLCharacter::Strafe(float Value)
{
	if (XLCharacterCan::Strafe(this))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AXLCharacter::Turn(float Direction)
{
	if (XLCharacterCan::Turn(this))
	{
		AddControllerYawInput(Direction * MovementComponent->BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AXLCharacter::Look(float Direction)
{
	if (XLCharacterCan::LookUp(this))
	{
		AddControllerPitchInput(Direction * MovementComponent->BaseLookRate * GetWorld()->GetDeltaSeconds());
	}
}

void AXLCharacter::Jump()
{
	if (XLCharacterCan::Jump(this))
	{
		SetActionState(EActionState::Jumping);
		Super::Jump();
	}
}
void AXLCharacter::Landed(const FHitResult& Hit)
{
	ActionState = EActionState::None;
	Super::Landed(Hit);
}

void AXLCharacter::StartSprint()
{
	if (XLCharacterCan::StartSprint(this))
	{
		ActionState = EActionState::Sprinting;
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * 2;
	}
}
void AXLCharacter::StopSprint()
{
	if (XLCharacterCan::StopSprint(this))
	{
		ActionState = EActionState::None;
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed / 2;
	}
}

void AXLCharacter::StartAttack()
{
	//CharacterWeapon->StartAttack();
}
void AXLCharacter::StopAttack()
{
	//CharacterWeapon->StopAttack();
}

void AXLCharacter::Reload()
{
	//CharacterWeapon->Reload();
}

void AXLCharacter::Melee()
{
	//CharacterWeapon->Melee();
}

void AXLCharacter::StartAbility(int32 Ability)
{
	if (XLCharacterCan::StartAbility(this))
	{
		CharacterAbilities->ActivateAbility(Ability);
	}
}
void AXLCharacter::StopAbility(int32 Ability)
{
	if (XLCharacterCan::StopAbility(this))
	{
		CharacterAbilities->DeactivateAbility(Ability);
	}
}


float AXLCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	AXLPlayerController* MyPC = Cast<AXLPlayerController>(Controller);
	if (CharacterResources->CurrentHealth <= 0.f)
	{
		return 0.f;
	}

	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (CharacterResources->CurrentShield > 0)
	{
		float temp = ActualDamage - CharacterResources->CurrentShield;
		CharacterResources->CurrentShield = CharacterResources->CurrentShield - ActualDamage;
		if (temp > 0)
		{
			ActualDamage = temp;
		}
	}
	else
	{
		//Need to check armor and reduce damage accordingly based on damage type.
		CharacterResources->CurrentHealth = CharacterResources->CurrentHealth - ActualDamage;
		if (CharacterResources->CurrentHealth <= 0)
		{
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{
			PlayHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		}
		MakeNoise(1.0f, EventInstigator ? EventInstigator->GetPawn() : this);
	}
	return ActualDamage;
}

bool AXLCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (!XLCharacterCan::Die(this))
	{
		return false;
	}

	CharacterResources->CurrentHealth = FMath::Min(0.0f, CharacterResources->CurrentHealth);

	// if this is an environmental death then refer to the previous killer so that they receive credit (knocked into lava pits, etc)
	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	Killer = GetDamageInstigator(Killer, *DamageType);

	AController* const KilledPlayer = (Controller != NULL) ? Controller : Cast<AController>(GetOwner());
	//GetWorld()->GetAuthGameMode<ASSGameMode>()->Killed(Killer, KilledPlayer, this, DamageType);

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);
	return true;
}

void AXLCharacter::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (HealthState == EHealthState::Dying)
	{
		return;
	}

	HealthState = EHealthState::Dying;
	PlayHit(KillingDamage, DamageEvent, PawnInstigator, DamageCauser);

	// cannot use IsLocallyControlled here, because even local client's controller may be NULL here
	/*if (CharacterEffects->GetDeathSound() && GetMesh() && GetMesh()->IsVisible())
	{
		UGameplayStatics::PlaySoundAtLocation(this, CharacterEffects->GetDeathSound(), GetActorLocation());
	}*/

	// remove all weapons
	//DestroyInventory();
	DetachFromControllerPendingDestroy();
	StopAllAnimMontages();

	/*if (RunLoopAC)
	{
	RunLoopAC->Stop();
	}*/

	// Death anim
	//float DeathAnimDuration = PlayAnimMontage(CharacterEffects->GetDeathAnim());
	//CharacterWeapon->SetLifeSpan(2.1f);
	SetLifeSpan(2.1f);
}

void AXLCharacter::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	// play the force feedback effect on the client player controller
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC && DamageEvent.DamageTypeClass)
	{
		/*USSDamageType *DamageType = Cast<USSDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
		if (DamageType && DamageType->HitForceFeedback)
		{
			PC->ClientPlayForceFeedback(DamageType->HitForceFeedback, false, "Damage");
		}*/
	}


	if (DamageTaken > 0.f)
	{
		//ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}


float AXLCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance)
	{
		return UseMesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void AXLCharacter::StopAnimMontage(class UAnimMontage* AnimMontage)
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance &&
		UseMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		UseMesh->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOut.GetBlendTime());
	}
}

void AXLCharacter::StopAllAnimMontages()
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		UseMesh->AnimScriptInstance->Montage_Stop(0.0f);
	}
}


void AXLCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AXLCharacter, ActionState);
}

bool AXLCharacter::ServerSetActionState_Validate(EActionState::Type State)
{
	return true;
}
void AXLCharacter::ServerSetActionState_Implementation(EActionState::Type State)
{
	SetActionState(State);
}
void AXLCharacter::SetActionState(EActionState::Type State)
{
	if (Role < ROLE_Authority)
	{
		ServerSetActionState(State);
	}
	else
	{
		ActionState = State;
	}
}

bool AXLCharacter::ServerSetCombatState_Validate(ECombatState::Type State)
{
	return true;
}
void AXLCharacter::ServerSetCombatState_Implementation(ECombatState::Type State)
{
	SetCombatState(State);
}
void AXLCharacter::SetCombatState(ECombatState::Type State)
{
	if (Role < ROLE_Authority)
	{
		ServerSetCombatState(State);
	}
	else
	{
		CombatState = State;
	}
}

bool AXLCharacter::ServerSetHealthState_Validate(EHealthState::Type State)
{
	return true;
}
void AXLCharacter::ServerSetHealthState_Implementation(EHealthState::Type State)
{
	SetHealthState(State);
}
void AXLCharacter::SetHealthState(EHealthState::Type State)
{
	if (Role < ROLE_Authority)
	{
		ServerSetHealthState(State);
	}
	else
	{
		HealthState = State;
	}
}

bool AXLCharacter::ServerSetMovementState_Validate(EMovementState::Type State)
{
	return true;
}
void AXLCharacter::ServerSetMovementState_Implementation(EMovementState::Type State)
{
	SetMovementState(State);
}
void AXLCharacter::SetMovementState(EMovementState::Type State)
{
	if (Role < ROLE_Authority)
	{
		ServerSetMovementState(State);
	}
	else
	{
		MovementState = State;
	}
}

bool AXLCharacter::ServerSetPostureState_Validate(EPostureState::Type State)
{
	return true;
}
void AXLCharacter::ServerSetPostureState_Implementation(EPostureState::Type State)
{
	SetPostureState(State);
}
void AXLCharacter::SetPostureState(EPostureState::Type State)
{
	if (Role < ROLE_Authority)
	{
		ServerSetPostureState(State);
	}
	else
	{
		PostureState = State;
	}
}