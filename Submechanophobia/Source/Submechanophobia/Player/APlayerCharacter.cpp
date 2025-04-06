// Fill out your copyright notice in the Description page of Project Settings.

#include "Submechanophobia/Player/APlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Submechanophobia/Enemies/Enemy.h"
#include "Submechanophobia/Weapons/WeaponBase.h"
 



// Sets default values
AAPlayerCharacter::AAPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,500.0f,0.0f);
	// load our animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAttackMontageObject(
		TEXT("/Script/Engine.AnimMontage'/Game/TEST/Enemy/Test_Enemy_Stuff/Anims/Shooting_Montage.Shooting_Montage'")
	);

	if (MeleeAttackMontageObject.Succeeded())
	{
		MeleeAttackMontage = MeleeAttackMontageObject.Object;
	}
}

// Called when the game starts or when spawned
void AAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

// Called every frame
void AAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AAPlayerCharacter::StartJump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AAPlayerCharacter::StopJump);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::FireWeapon);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Look);
		EnhancedInput->BindAction(SwapToPrimaryAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::SwapWeaponPrimary);
		EnhancedInput->BindAction(SwapToSecondaryAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::SwapWeaponSecondary);
		EnhancedInput->BindAction(SwapToSecondaryAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Reload);

	}
}




void AAPlayerCharacter::StartJump()
{
	bPressedJump = true;
}

void AAPlayerCharacter::StopJump()
{
	bPressedJump = false;
}

void AAPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirecton = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirecton, DirectionValue.Y);

	const FVector RightDirecton = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirecton, DirectionValue.X);
}

void AAPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}



void AAPlayerCharacter::FireWeapon()
{
	if (currentWeapon)
	{
		if (currentWeapon->magazineAmmo > 0 )
		{
			FHitResult* HitResult = RayCast();
			currentWeapon->magazineAmmo--;
			UGameplayStatics::ApplyDamage(HitResult->GetActor(),currentWeapon->damage,nullptr,this,nullptr); 
		}
	}
	
}

void AAPlayerCharacter::SwapWeaponPrimary()
{
	if (PrimaryWeapon && currentWeapon != PrimaryWeapon)
	{
		if (SecondaryWeapon)
			SecondaryWeapon->AltweaponMesh->SetVisibility(false);
		
		PrimaryWeapon->weaponMesh->SetVisibility(true);
		currentWeapon =  PrimaryWeapon;
	}
	
	
}

void AAPlayerCharacter::SwapWeaponSecondary()
{
	if (SecondaryWeapon && currentWeapon != SecondaryWeapon)
	{
		if (PrimaryWeapon)
			PrimaryWeapon->weaponMesh->SetVisibility(false);
		
		SecondaryWeapon->AltweaponMesh->SetVisibility(true);
		currentWeapon =  SecondaryWeapon;
	}
	
}

void AAPlayerCharacter::Reload()
{
	if (currentWeapon)
	{
		if (currentWeapon->magazineAmmo < currentWeapon->magazineCapacity)
		{
			currentWeapon->magazineAmmo += currentWeapon->magazineAmmo - currentWeapon->reserveAmmo;
		}
	}
}

FHitResult* AAPlayerCharacter::RayCast()
{
	
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = Camera->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();
	FVector EndTrace =  (ForwardVector * 5000.f) + StartTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	CQP->bIgnoreBlocks = false;
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Camera, *CQP))
	{
		//PlayAnimMontage(MeleeAttackMontage);
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 255), true);
		return HitResult; 
	}
	return  HitResult;
}



void AAPlayerCharacter::AddWeapon( AUWeaponBase* weapon)
{
	
	
	
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	
	if (weapon->isPrimary)
	{
		if (PrimaryWeapon != nullptr)
		{
			PrimaryWeapon->weaponMesh->SetVisibility(true);
			PrimaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PrimaryWeapon->IsPickedUp = false;
		}
		
		PrimaryWeapon = weapon;
		PrimaryWeapon->weaponMesh->SetVisibility(false);
	}
	else if (!weapon->isPrimary)
	{
		if (SecondaryWeapon != nullptr)
		{
			SecondaryWeapon->AltweaponMesh->SetVisibility(true);
			SecondaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			SecondaryWeapon->IsPickedUp = false;
		}
		
		SecondaryWeapon = weapon;
		SecondaryWeapon->AltweaponMesh->SetVisibility(false);
		
	}
	
	weapon->AttachToComponent(GetMesh(),AttachRules,TEXT("WeaponSocket"));
}



