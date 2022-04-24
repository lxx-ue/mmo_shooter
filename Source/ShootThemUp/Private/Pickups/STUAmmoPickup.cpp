// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All)

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn) //можно ли подобрать
{
    //есть ли компонент здоровья и оружия
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (!WeaponComponent) return false;
    //пытаемся добавить патроны, если получается - функция возвращает истину и пикап подбирается
	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}