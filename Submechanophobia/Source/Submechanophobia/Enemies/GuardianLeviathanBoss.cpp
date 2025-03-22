// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardianLeviathanBoss.h"
#include "GuardianLeviathanAIC.h"
#include "GuardianLeviathanBoss.h"
#include "Submechanophobia/Player/APlayerCharacter.h"

AGuardianLeviathanBoss::AGuardianLeviathanBoss()
{
	Health = 100.0f;
	CurrentStage = 1;
}

void AGuardianLeviathanBoss::BeginPlay()
{
	Super::BeginPlay();

	BossAIC = Cast<AGuardianLeviathanAIC>(GetController());

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
    GetOverlappingActors(Players, AAPlayerCharacter::StaticClass());

    for (AActor* Player : Players)
    {
        //Cast<AAPlayerCharacter>(Player)->TakeDamage(10.0f, FDamageEvent(), GetController(), this);
    }

}

void AGuardianLeviathanBoss::StopFireBreath()
{
    GetWorldTimerManager().ClearTimer(FireTimerHandle);

}

void AGuardianLeviathanBoss::StartScreech()
{
    TArray<AActor*> Players;
    GetOverlappingActors(Players, AAPlayerCharacter::StaticClass());

    for (AActor* Player : Players)
    {
        //Cast<AAPlayerCharacter>(Player)->TakeDamage(15.0f, FDamageEvent(), GetController(), this);
       // Cast<AAPlayerCharacter>(Player)->ApplyStun(1.0f);
    }

}

void AGuardianLeviathanBoss::LungeAtTarget(AActor* Target)
{
    FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
   // LaunchCharacter(Direction * 1000, true, true);

}


