// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHUD.generated.h"

class USTUBaseWidget;
class USTUPlayerHUDWidget;

UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//оверрайд функции рисования хада
    virtual void DrawHUD() override;

	void addKill(FString killerName, int32 killerTeam, FString victimName, int32 victimTeam, bool HeadShot);

	UPROPERTY()
		class USTUPlayerHUDWidget* HUDWidget = nullptr;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PlayerWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PauseWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> GameOverWidgetClass;
	virtual void BeginPlay() override;
private:
	UPROPERTY()
		TMap<ESTUMatchState, USTUBaseWidget*> GameWidgets;
	UPROPERTY()
		USTUBaseWidget* CurrentWidget = nullptr;
	//рисование прицела
    void DrawCrossHair();
	void OnMatchStateChanged(ESTUMatchState State);

	USTUPlayerHUDWidget* PlayerHUD;
};
