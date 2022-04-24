// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
public: 
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;
    virtual void Zoom(bool Enabled) override;

protected:
    //скорость стрельбы
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;
    //разброс
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;
    //урон
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        float FOVZomAngle = 50.0f;

    virtual void BeginPlay() override;
    virtual void MakeShot() override;
    virtual bool GetTracedData(FVector& TraceStart, FVector& TraceEnd) const override;
    
 private:
    FTimerHandle ShotTimerHandle;
     UPROPERTY()
     UNiagaraComponent* MuzzleFXComponent;

     UPROPERTY()
         UAudioComponent* FireAudioComponent;

     void MakeDamage(const FHitResult& HitResult); //нанесени урона
     void InitFX();
     void SetFXActive(bool IsActive);
     void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

     AController* GetController() const;
     float DefaultCameraFOV = 90.0f;
};
