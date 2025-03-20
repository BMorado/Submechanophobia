// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/GuardianLeviathanBoss.h"
#include "Enemies/GuardianLeviathanAIC.h"
#include "GuardianLeviathanBoss.h"

AGuardianLeviathanBoss::AGuardianLeviathan()
{
	Health = 100.0f;
	CurrentStage = 1;
}

void AGuardianLeviathanBoss::BeginPlay()
{
	Super::BeginPlay();

	BossAIC = Cast<AGuardianLeviathanAIC>(GetController());

}

void AGuardianLeviathanBoss::TakeDamage(float DamageAmount)
{
    Health -= DamageAmount;

    if (Health <= 66.0f && CurrentStage == 1)
    {
        EnterNextStage();
    }
    else if (Health <= 33.0f && CurrentStage == 2)
    {
        EnterNextStage();
    }

}

void AGuardianLeviathanBoss::EnterNextStage()
{
    CurrentStage++;

}

void AGuardianLeviathanBoss::StartFireBreath()
{
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGuardianLeviathanBoss::ApplyFireDamage, 0.5f, true, 0.0f);
    GetWorldTimerManager().SetTimer(StopFireHandle, this, &AGuardianLeviathanBoss::StopFireBreath, 5.0f, false);

}

void AGuardianLeviathanBoss::ApplyFireDamage()
{

    TArray<AActor*> Players;
    GetOverlappingActors(Players, APlayerCharacter::StaticClass());

    for (AActor* Player : Players)
    {
        Cast<APlayerCharacter>(Player)->TakeDamage(10.0f, FDamageEvent(), GetController(), this);
    }

}

void AGuardianLeviathanBoss::StopFireBreath()
{
    GetWorldTimerManager().ClearTimer(FireTimerHandle);

}

void AGuardianLeviathanBoss::StartScreech()
{
    TArray<AActor*> Players;
    GetOverlappingActors(Players, APlayerCharacter::StaticClass());

    for (AActor* Player : Players)
    {
        Cast<APlayerCharacter>(Player)->TakeDamage(15.0f, FDamageEvent(), GetController(), this);
        Cast<APlayerCharacter>(Player)->ApplyStun(1.0f);
    }

}

void AGuardianLeviathanBoss::LungeAtTarget(AActor* Target)
{
    FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    LaunchCharacter(Direction * 1000, true, true);

}


