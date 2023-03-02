// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/STUGameModeTeamDeathMatch.h"
#include "Kismet/GameplayStatics.h"
#include "AI/BotSpawner.h"
#include <ShootThemUp/Public/Player/STUPlayerState.h>

void ASTUGameModeTeamDeathMatch::CreateTeamsInfo() // распределяем ботов по командам
{
    if (!GetWorld()) return;
    int32 TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;
        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID).color);
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? PlayersName : Super::GetRandomBotName());
        PlayerState->SetIsPlayer(Controller->IsPlayerController());
        SetPlayerColor(Controller);
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FSTUColors ASTUGameModeTeamDeathMatch::DetermineColorByTeamID(int32 TeamID) const
{
    return TeamID == 1 ? PlayerTeamColor : EnemyTeamColor;
}
