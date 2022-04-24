// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
    check(WeaponMesh); //не задан меш
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero")); //если дефолтные патроны меньше 0
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero")); //если дефолтные обоймы меньше 0
    CurrentAmmo = DefaultAmmo;
}

//функии для перегрузки
void ASTUBaseWeapon::StartFire() 
{
    
}

void ASTUBaseWeapon::StopFire()
{
    
}

void ASTUBaseWeapon::MakeShot() 
{
    
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const  //куда направлена камера
{
    const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if (!STUCharacter) return false;

    if (STUCharacter->IsPlayerControlled())
    {
        const auto Controller = STUCharacter->GetController<APlayerController>();
        if (!Controller)return false;
        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = GetMuzzleWorldLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }

    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const //получить сокет откуда пули летят
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTracedData(FVector& TraceStart, FVector& TraceEnd) const  //начало и конец траектории пули
{
    FVector ViewLocation; 
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation; //старт трейса - дуло
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance; //конец трейса - дистанция помноженная на прицел
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) //выстрел
{
    if (!GetWorld()) return; 
    FCollisionQueryParams CollisionParams; 
    CollisionParams.AddIgnoredActor(GetOwner()); //классы которые игнорируем 
    CollisionParams.bReturnPhysicalMaterial = true;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams); //лайнтрейс выстрела
}

void ASTUBaseWeapon::DecreaseAmmo() //уменьшить кол-во патронов
{
    if (CurrentAmmo.Bullets == 0) //патронов нет
    {
        //UE_LOG(LogBaseWeapon, Display, TEXT("------clip is empty------"));
        return;
    }
    CurrentAmmo.Bullets--; //уменьшить кол-во
    //LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty()) //если обойма пуста и патроны еще есть
    {
        StopFire(); //прекратить стрельбу
        OnClipEmpty.Broadcast(this); //вызов делегата
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const //нет патронова
{
    //обоймы не бесконечные и обойм больше нет
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && CurrentAmmo.Bullets == 0; //TODO: заменить на функцию
}

bool ASTUBaseWeapon::IsClipEmpty() //обойма пуста
{
    return CurrentAmmo.Bullets == 0; //потронов нет
}

void ASTUBaseWeapon::ChangeClip()  //перезарядить
{
    if (!CurrentAmmo.Infinite) //если обоймы не бесконечные
    {
        if (CurrentAmmo.Clips == 0) //если обоймы кончились
        {
            //UE_LOG(LogBaseWeapon, Display, TEXT("------no more clips------"));
            return;
        }
        CurrentAmmo.Clips--; //уменьшить количество обойм
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets; //восстановить патроны
    //UE_LOG(LogBaseWeapon, Display, TEXT("------change clip------"));
}

bool ASTUBaseWeapon::CanReload() const //может ли перезарядиться
{
    //если текущих патронов меньше дефолтных и есть обоймы
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ASTUBaseWeapon::LogAmmo() //логирование
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / "; //патроны
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips); //обоймы
    //UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo); //вывод в лог
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    //патроны бесконечны, полный боезопас или ноль обойм в пикапе 
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)
        return false;

    if (IsAmmoEmpty()) //нет патронов совсем
    {
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips) //обойм меньше максимума
    {
        const auto NextClipAmount = CurrentAmmo.Clips + ClipsAmount;
        if (DefaultAmmo.Clips - NextClipAmount >= 0) //не можем покрыть весь боезапас
        {
            CurrentAmmo.Clips = NextClipAmount;
        }
        else //можем
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        }
    }
    else //не хватает тольк пуль
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    }
    return true;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX() //fx у дула
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,              //
                                                 WeaponMesh,            //
                                                 MuzzleSocketName,      //
                                                 FVector::ZeroVector,   //
                                                 FRotator::ZeroRotator, //
                                                 EAttachLocation::SnapToTarget, true);
}