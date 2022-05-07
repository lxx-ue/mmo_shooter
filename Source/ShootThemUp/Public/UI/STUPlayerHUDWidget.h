// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

class UProgressBar;
class UVerticalBox;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
    GENERATED_BODY()

public:
    //процент хп
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    //получить иконки
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

    //инфа о патронах
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    //жив ли игрок
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    //находится ли в режиме наблюдателя
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpecraring() const;

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();

    UFUNCTION(BlueprintCallable, Category = "UI")
        int32 GetKillsNum() const;

    UPROPERTY(meta = (BindWidget))
        UVerticalBox* VerticalBox_killbar;

    void ClearVerticalBox(FString killer, FString victim);
    void test();

protected:
    UPROPERTY(meta = (BindWidget))
        UProgressBar* HealthProgressBar;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
        UWidgetAnimation* DamageAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        float PercentColorThreshold = 0.3f; // с какого процента цвет бара меняется

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        FLinearColor GoodColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        FLinearColor BadColor = FLinearColor::Red;

    virtual void NativeOnInitialized() override;

private:
    void OnHealthChanged(float Health, float HealthData);
    void OnNewPawn(APawn* NewPawn);
    void UpdateHealthBar();
};