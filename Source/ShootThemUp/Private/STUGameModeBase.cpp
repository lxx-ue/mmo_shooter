// Fill out your copyright notice in the Description page of Project Settings.

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "Player/STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "Components/STUWeaponComponent.h"
#include "EngineUtils.h"
#include "STUGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase() {
    //задаем базовые классы гейм мода
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    if (GetWorld())
    {
        const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
        if (!STUGameInstance) return;
        const auto StartedLevel = STUGameInstance->GetStartupLevel();
    }
}

void ASTUGameModeBase::StartPlay() 
{
    Super::StartPlay();
    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    RoundsNum = STUGameInstance->GetRounds();
    PlayersNum = STUGameInstance->GetPlayersNum();
    RoundTime = STUGameInstance->GetRoundTime();
    PlayersName = STUGameInstance->GetPlayersName();
    SpawnBots();
    CreateTeamsInfo();
    CurrentRound = 1;
    StartRound();
    SetMatchState(ESTUMatchState::InProgress);
    defaultHUD = Cast<ASTUGameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;
    for (int32 i = 0; i < PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        FVector loc(0, 0, 0);
        FRotator rot(0, 0, 0);
        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, loc, rot, SpawnInfo);
        RestartPlayer(STUAIController);
    }
}

void ASTUGameModeBase::StartRound() 
{
    RoundCountDown = RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if (!GetWorld()) return;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
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
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? PlayersName : GetRandomBotName());
        SetPlayerColor(Controller);
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    return TeamID == 1 ? PlayerTeamColor : EnemyTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;
    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if (!Character) return;
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!PlayerState) return;
    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    VictimPlayerState->AddDeath();
    StartRespawn(VictimController);
    if (KillerPlayerState)
        if (KillerPlayerState->GetTeamID() == VictimPlayerState->GetTeamID())
            KillerPlayerState->AddKillTeammate();
        else
            KillerPlayerState->AddKill();
    defaultHUD->addKill(KillerPlayerState->GetPlayerName(), KillerPlayerState->GetTeamID(),
        VictimPlayerState->GetPlayerName(), VictimPlayerState->GetTeamID());
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAviable = RoundCountDown > MinRoundTimeForRespawn + RespawnTime;
    if (!RespawnAviable) return;
    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
    if (!RespawnComponent) return;
    RespawnComponent->Respawn(RespawnTime);
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

void ASTUGameModeBase::GameOver()
{
    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }
    SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMathcStateChanged.Broadcast(MatchState);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet)
    {
        StopAllFire();
        SetMatchState(ESTUMatchState::Pause);
    }
    return PauseSet;
    
}

bool ASTUGameModeBase::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetMatchState(ESTUMatchState::InProgress);
    }
    return PauseCleared;
}

void ASTUGameModeBase::StopAllFire()
{
    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Pawn);
        if (!WeaponComponent) continue;
        WeaponComponent->StopFire();
        WeaponComponent->Zoom(false);
    }
}

FString ASTUGameModeBase::GetRandomBotName() const
{
    return botnames[FMath::RandRange(0, botnames.Num()-1)];
}