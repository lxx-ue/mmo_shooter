// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 USTUGameDataWidget::GetTotalRoundNum() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetRoundsNum() : 0;
}

int32 USTUGameDataWidget::GetRoundSecondsRemaining() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

int32 USTUGameDataWidget::GetPlayerTeamPoints() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetPlayerTeamPoints() : 0;
}

int32 USTUGameDataWidget::GetEnemiesTeamPoints() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetEnemiesTeamPoints() : 0;
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
    return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
    return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}