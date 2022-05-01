// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponComponent();

	virtual void StartFire(); //начать стрельбу
    void StopFire(); //закончить стрельбу
    virtual void NextWeapon(); //переключить оружие
    void Reload(); //перезарядка

    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);
    bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType);

    void Zoom(bool Enabled);
    void ChangeClip(); //смена обоймы

protected:
	//переменная базовый класс оружия - массив структур (оружие + аним монтаж перезарядки)
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

	//сокет куда присоединять текущее оружие
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

   	//сокет куда присоединять второе оружие
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    //аним монтаж взятия переключения оружия
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    //массив оружий
    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    //ссылка на текущее оружие
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

   	int32 CurrentWeaponIndex = 0; //номер текущего оружия

	virtual void BeginPlay() override; //начало игры
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; //конец игры (чтобы оружия удалять со сцены)

    bool CanFire() const;  //может ли стрелять
    bool CanEquip() const; //может ли сменить оружие
    void EquipWeapon(int32 WeaponIndex); // достаешь оружие

private:	
    //текущий аним монтаж перезарядки
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool EquipAnimInProgress = false; //меняешь оружие щас
    bool ReloadAnimInProgress = false; //перезаряжаешься щас

	void SpawnWeapons(); //спавн оружия
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName); //присоединяешь оружие к сокету
	void PlayAnimMontage(UAnimMontage* Animation); //старт анимации
    void InitAnimations(); //инит анимаций
    void OnEquipFinished(USkeletalMeshComponent* MeshComp); //сменил оружие
    void OnReloadFinished(USkeletalMeshComponent* MeshComp); //перезарядил оружие
    void OnClipEmpty(ASTUBaseWeapon * AmmoEmptyWeapon);                        //когда обойма пуста
    bool CanReload() const; //может ли перезарядиться
};
