// Fill out your copyright notice in the Description page of Project Settings.

#include "Submechanophobia/Player/APlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "IPropertyTable.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Submechanophobia/Weapons/WeaponBase.h"
#include "UniversalObjectLocators/UniversalObjectLocatorUtils.h"



// Sets default values
AAPlayerCharacter::AAPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	
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
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::RayCast);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Look);
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







void AAPlayerCharacter::RayCast()
{
	
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = Camera->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();
	FVector EndTrace =  (ForwardVector * 5000.f) + StartTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *CQP))
	{
		PlayAnimMontage(MeleeAttackMontage);
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 255), true);

		if (HitResult->GetActor() != nullptr)
		{
			//if actor has health?
			//then get weapon if weapon not in player
			//or weapon.getdamage
			//Check if health <0 play death montage and delete actor. 
			HitResult->GetActor()->Destroy();
			
		}
	}

}



void AAPlayerCharacter::AddWeapon(TSubclassOf<AUWeaponBase> weapon)
{
	FVector SpawnLocation = Camera->GetForwardVector() + GetActorLocation();
	FRotator SpawnRotation = Camera->GetComponentRotation();
	FActorSpawnParameters SpawnInfo;
	
	AUWeaponBase* NewWeapon = GetWorld()->SpawnActor<AUWeaponBase>(weapon, SpawnLocation, SpawnRotation, SpawnInfo);
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	
	currentWeapon = NewWeapon->GetClass();
	
	NewWeapon->AttachToComponent(GetMesh(), AttachRules);  // Make sure you have a valid socket name like "WeaponSocket"


}

