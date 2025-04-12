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
	Camera->SetupAttachment(GetMesh(), TEXT("HeadCameraSocket")); // or TEXT("head")
	Camera->bUsePawnControlRotation = true;

	Camera->SetRelativeLocation(FVector::ZeroVector);
	Camera->SetRelativeRotation(FRotator::ZeroRotator);
	
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,500.0f,0.0f);
	// load our animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAttackMontageObject(
		TEXT("/Script/Engine.AnimMontage'/Game/TEST/Enemy/Test_Enemy_Stuff/Anims/Shooting_Montage.Shooting_Montage'")
	);

	if (MeleeAttackMontageObject.Succeeded())
	{
		MeleeAttackMontage = MeleeAttackMontageObject.Object;
	}
	HitResult = new FHitResult(0); 
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
		EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Reload);
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
	if (currentWeapon != nullptr && currentWeapon->magazineAmmo > 0 && canShoot)
	{
		RayCast(); 
		currentWeapon->magazineAmmo--;
		UGameplayStatics::ApplyDamage(HitResult->GetActor(),currentWeapon->damage,nullptr,this,nullptr);
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle,this,&AAPlayerCharacter::WeaponFireDelay,currentWeapon->fireRate,false); 
	}
	
}


void AAPlayerCharacter::WeaponFireDelay()
{
	canShoot = true ; 
}
//hides secondary weapon, makes primary weapon visible then sets primary to current weapon
void AAPlayerCharacter::SwapWeaponPrimary()
{
	if (PrimaryWeapon && currentWeapon != PrimaryWeapon)
	{
		if (SecondaryWeapon)
			SecondaryWeapon->weaponMesh->SetVisibility(false);
		
		PrimaryWeapon->weaponMesh->SetVisibility(true);
		currentWeapon =  PrimaryWeapon;
	}
	
	
}

//hides primary weapon, makes secondary weapon visible then sets secondary to current weapon
void AAPlayerCharacter::SwapWeaponSecondary()
{
	if (SecondaryWeapon && currentWeapon != SecondaryWeapon)
	{
		if (PrimaryWeapon)
			PrimaryWeapon->weaponMesh->SetVisibility(false);
		
		SecondaryWeapon->weaponMesh->SetVisibility(true);
		currentWeapon =  SecondaryWeapon;
	}
	
}

void AAPlayerCharacter::Reload()
{
	// Check if pointer isn't null and that we have a weapon
	if (currentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Mag  Pre-Reload: %u"),currentWeapon->magazineAmmo);
		
		// See if the number of "bullets" is below capacity
		if (currentWeapon->magazineAmmo < currentWeapon->magazineCapacity && currentWeapon->reserveAmmo != 0)
		{
			uint8 ReloadCalculation = (currentWeapon->reserveAmmo >= currentWeapon->magazineCapacity) ? (currentWeapon->magazineAmmo - currentWeapon->magazineCapacity) * -1 : currentWeapon->reserveAmmo;
			
			currentWeapon->magazineAmmo += ReloadCalculation;
			
			currentWeapon->reserveAmmo -= ReloadCalculation;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Current Mag  Post-Reload: %u"),currentWeapon->magazineAmmo);

}
	



void  AAPlayerCharacter::RayCast()
{
	canShoot = false;
	
	FVector StartTrace = Camera->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();
	FVector offset = FMath::VRand() * 2.0f;
	 
	FVector EndTrace = (currentWeapon->bulletSpread + ForwardVector * 5000.f) + StartTrace;
	
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	CQP->bIgnoreBlocks = false;
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Camera, *CQP))
	{
		//PlayAnimMontage(MeleeAttackMontage);
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 255), true);
		
	}
	
}



void AAPlayerCharacter::AddWeapon( AUWeaponBase* weapon)
{
	
	
	
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	//checks if the weapon being added is a Primary weapon
	if (weapon->isPrimary)
	{
		//see if we've already got a primary weapon
		if (PrimaryWeapon != nullptr)
		{
			//sets that weapon visible | Remove previous primary from player | Set flag false
			
			PrimaryWeapon->weaponMesh->SetVisibility(true);
			PrimaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PrimaryWeapon->IsPickedUp = false;
		}

		//new weapon is now set as primary and is hidden until player equips
		PrimaryWeapon = weapon;
		PrimaryWeapon->weaponMesh->SetVisibility(false);
		PrimaryWeapon->weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weapon->AttachToComponent(GetMesh(),AttachRules,TEXT("WeaponSocket_Primary"));

	}
	// same protocol as primary weapon performed for secondary type weapons
	else if (!weapon->isPrimary)
	{
		if (SecondaryWeapon != nullptr)
		{
			SecondaryWeapon->weaponMesh->SetVisibility(true);
			SecondaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			SecondaryWeapon->IsPickedUp = false;
		}
		
		SecondaryWeapon = weapon;
		SecondaryWeapon->weaponMesh->SetVisibility(false);
		SecondaryWeapon->weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weapon->AttachToComponent(GetMesh(),AttachRules,TEXT("WeaponSocket_Secondary"));

		
	}

	//attach weapon actor to player mesh socket 
}



