#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "HealthComponent.h"
#include "Animation/AnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GuardianLeviathanBoss.generated.h"

class UNiagaraComponent;

USTRUCT(BlueprintType)
struct SUBMECHANOPHOBIA_API FSpawnHole
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Movement")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Movement")
    FRotator Rotation;
};

UCLASS()
class SUBMECHANOPHOBIA_API AGuardianLeviathanBoss : public AEnemy
{
    GENERATED_BODY()

public:
    AGuardianLeviathanBoss();

protected:
    virtual void BeginPlay() override;

public:
    // --- Stage Tracking ---
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    //float Health = 100.0f;

    UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_CurrentStage, BlueprintReadWrite, Category = "Boss Stats")
    int CurrentStage = 1;

    UFUNCTION()
    void OnRep_CurrentStage();

    UFUNCTION(BlueprintCallable)
    void EnterNextStage();

    static float SharedHealth;
    static float MaxSharedHealth;

    UPROPERTY(ReplicatedUsing = OnRep_SharedHealth)
    float ReplicatedSharedHealth = 100.f;

    // RepNotify function to update locally when replicated
    UFUNCTION()
    void OnRep_SharedHealth();

    UFUNCTION(BlueprintCallable)
    static float GetSharedHealth();


    static AGuardianLeviathanBoss* PrimaryBoss; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
    bool bIsPrimaryBoss = false;

    void OnBossDamaged(float);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable)
    void ApplySharedDamage(float Amount);


    // --- Anim Montages ---
    UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, Category = "Boss|Animation")
    UAnimMontage* FireMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Animation")
    UAnimMontage* ScreechMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Animation")
    UAnimMontage* LungeMontage;

    UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, Category = "Boss|Animation")
    UAnimMontage* TransitionMontage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Hitboxes")
    UCapsuleComponent* FireDamageHitbox;

    // --- Animation Events ---
    FOnMontageEnded MontageEndDelegate;

    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    // --- Timer Handles ---
    FTimerHandle FireTimerHandle;
    FTimerHandle StopFireHandle;

    // --- Attack Calls (for BT or Blueprint) ---
    UFUNCTION(BlueprintCallable)
    void FireAttack();

    void ApplyScreechDamage();
    UFUNCTION(BlueprintCallable)
    void ScreechAttack();

    void ApplyLungeDamage(); // Called on impact or during animation
    UFUNCTION(BlueprintCallable)
    void LungeAttack();

    UFUNCTION(BlueprintCallable)
    void TransitionOut();

    UFUNCTION(BlueprintCallable)
    void PlayTransitionIn();

    // --- Internal Helpers ---
    void ApplyFireDamage();
    void StopFireBreath();

    UFUNCTION(BlueprintCallable)
    void MoveToNewHole();

    
    // --- Screech Sound ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Audio")
    USoundBase* ScreechSound;

    // Static tracking of occupied holes (shared across all serpents)
    static TArray<int32> OccupiedHoleIndices;

    // --- New Hole Transform Setup ---
    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Boss|Movement")
    TArray<FSpawnHole> HoleTransforms;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Movement")
    int32 LastHoleIndex = -1;

    int32 FindAvailableHoleIndex();

    UPROPERTY()
    UNiagaraComponent* FlameEffect1;

    UPROPERTY()
    UNiagaraComponent* FlameEffect2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|UI")
    TSubclassOf<class UUserWidget> WinWidgetClass;

    //REPLICATION STUFF ADDED

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    UFUNCTION(Server, Reliable)
    void Server_FireAttack();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayFireAttack();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayTransitionIn();
   
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayTransitionOut();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnBossDefeated();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_SpawnSerpent(FVector Location, FRotator Rotation, int32 Stage, int32 HoleIndex);
   

};