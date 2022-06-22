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
            GameMode->OnMathcStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
    }
    if (ResetLevelButton)
        ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::GameOver)
        UpdatePlayersStat();
}

void USTUGameOverWidget::UpdatePlayersStat()
{
    if (!GetWorld() || !PlayerStatBox || !EnemyStatBox) return;
    bool team = true;
    int head = 0;
    TArray<USTUPlayerStatRowWidget*> uArray;
    TArray<USTUPlayerStatRowWidget*> eArray;
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
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
        if (team)
        {
            YourTeamColor = PlayerState->GetTeamColor();
            uArray.Add(PlayerStatRowWidget);
            team = !team;
        }
        else
        {
            EnemyTeamColor = PlayerState->GetTeamColor();
            eArray.Add(PlayerStatRowWidget);
            team = !team;
        }
    }
    uArray.Sort([](const USTUPlayerStatRowWidget& ip1, const USTUPlayerStatRowWidget& ip2) { return  ip1.kills > ip2.kills; });
    eArray.Sort([](const USTUPlayerStatRowWidget& ip1, const USTUPlayerStatRowWidget& ip2) { return  ip1.kills > ip2.kills; });
    uArray.Sort([](const USTUPlayerStatRowWidget& ip1, const USTUPlayerStatRowWidget& ip2) { return  ip1.deaths < ip2.deaths; });
    eArray.Sort([](const USTUPlayerStatRowWidget& ip1, const USTUPlayerStatRowWidget& ip2) { return  ip1.deaths < ip2.deaths; });
    for (int i = 0; i < uArray.Num(); i++)
    {
        PlayerStatBox->AddChild(uArray[i]);
        EnemyStatBox->AddChild(eArray[i]);
    }
    WidgetDraw = true;
}

void USTUGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}