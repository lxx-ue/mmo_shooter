// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASTULauncherWeapon::StartFire() //������ ��������
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot() //����������
{
    if (!GetWorld())
        return; //��� �� ������ - �������

    if (IsAmmoEmpty())
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTracedData(TraceStart, TraceEnd))
        return; //����� ������

    FHitResult HitResult; //��������� ������
    MakeHit(HitResult, TraceStart, TraceEnd); //���������

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd; //����������� �� ������
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal(); //����������

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation()); //��������� ������
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform); //����� ������
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