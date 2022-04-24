// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;

    virtual void StartFire(); //начало стрельбы
    virtual void StopFire(); //конец стрельбы

    void ChangeClip(); //сменить обойму
    bool CanReload() const; //можно ли сменить обойму

    FWeaponUIData GetUIData() const { return UIData; } //получить иконки
    FAmmoData GetAmmoData() const { return CurrentAmmo; } //инфа о боеприпасах

    bool TryToAddAmmo(int32 ClipsAmount);
    bool IsAmmoEmpty() const; //если патронов нет
    bool IsAmmoFull() const;

    virtual void Zoom(bool Enabled) {}

protected:
	//меш оружия
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    //сокет откуда пули летят
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    //дистанция стрельбы
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

    //дефолтные патроны
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 3, false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* MuzzleFX;

    //иконки
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
        USoundCue* FireSound;

	virtual void BeginPlay() override;
    virtual void MakeShot();
    virtual bool GetTracedData(FVector& TraceStart, FVector& TraceEnd) const;

    //вспомогательные функции выстрела
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const; 
    FVector GetMuzzleWorldLocation() const;
    // попадание
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd); //выстрел

    void DecreaseAmmo(); //уменьшить кол-во патронов
    bool IsClipEmpty(); //если обойм нет
    void LogAmmo(); //вывести статы в лог
    
    UNiagaraComponent* SpawnMuzzleFX();


private:
    FAmmoData CurrentAmmo;  //текущие патроны
};
