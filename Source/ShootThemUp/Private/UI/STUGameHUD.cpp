// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/STUBaseWidget.h"
#include "STUGameModeBase.h"
#include "UI/STUPlayerHUDWidget.h"
#include <Runtime/Core/Public/Misc/OutputDeviceNull.h>

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD() 
{
    Super::DrawHUD();
}

void ASTUGameHUD::BeginPlay() 
{
    Super::BeginPlay();

    GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<USTUBaseWidget>(GetWorld(), PlayerWidgetClass));
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass));    

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;
        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMathcStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
        }
    }

    for (auto GameWidgetPair : GameWidgets)
    {
        if (GameWidgetPair.Key == ESTUMatchState::InProgress)
        {
            HUDWidget = Cast<USTUPlayerHUDWidget>(GameWidgetPair.Value->GetClass());
        }
    }

    PlayerHUD = Cast<USTUPlayerHUDWidget>(PlayerWidgetClass);
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }
}

void ASTUGameHUD::DrawCrossHair()
{
    //рисуем прицел
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

void ASTUGameHUD::addKill(FString killerName, int32 killerTeam, FString victimName, int32 victimTeam, bool HeadShot)
{
    FOutputDeviceNull OutputDeviceNull;
    const FString CmdAndParams = FString::Printf(TEXT("AddKillBar %s %d %s %d %d"), *killerName, killerTeam, *victimName, victimTeam, (int)HeadShot);
        this->CallFunctionByNameWithArguments(*CmdAndParams, OutputDeviceNull, nullptr, true);
}