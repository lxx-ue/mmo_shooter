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
    //������� ��
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    //�������� ������
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

    //���� � ��������
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    //��� �� �����
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    //��������� �� � ������ �����������
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
        float PercentColorThreshold = 0.3f; // � ������ �������� ���� ���� ��������

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