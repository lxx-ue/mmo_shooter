// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASTULauncherWeapon::StartFire() //начать стрельбу
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot() //выстрелить
{
    if (!GetWorld())
        return; //мир не создан - выходим

    if (IsAmmoEmpty())
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTracedData(TraceStart, TraceEnd))
        return; //трейс сделан

    FHitResult HitResult; //результат трейса
    MakeHit(HitResult, TraceStart, TraceEnd); //пападание

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd; //приземлился ли патрон
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal(); //напраление

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation()); //параметры спавна
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform); //спавн ракеты
    if (Projectile) 
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
    DecreaseAmmo();
    SpawnMuzzleFX();
    UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
}