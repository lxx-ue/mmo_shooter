// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "UI/STUPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "STUUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMathcStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
        }
    }
    if (ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
    }
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::GameOver)
    {
        UpdatePlayersStat();
    }
}

void USTUGameOverWidget::UpdatePlayersStat()
{
    if (!GetWorld() || !PlayerStatBox || !PlayerStatBox_1) return;
    PlayerStatBox->ClearChildren();
    PlayerStatBox_1->ClearChildren();
    bool team = true;
    int head = 0;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        if (head > 2) break;
        const auto Controller = It->Get();
        if (!Controller) continue;
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;
        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget) continue;
        PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());
        if (team)
        {
            PlayerStatBox->AddChild(PlayerStatRowWidget);
            team = !team;
        }
        else
        {
            PlayerStatBox_1->AddChild(PlayerStatRowWidget);
            team = !team;
        }
        head++;
    }

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;
        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathsNum()));
        //PlayerStatRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
        //PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

        if (team)
        {
            PlayerStatBox->AddChild(PlayerStatRowWidget);
            team = !team;
        }
        else
        {
            PlayerStatBox_1->AddChild(PlayerStatRowWidget);
            team = !team;
        }

    }
}

void USTUGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}