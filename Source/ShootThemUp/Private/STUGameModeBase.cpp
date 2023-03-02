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
#include "AI/BotSpawner.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase() {
    //задаем базовые классы гейм мода
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    if (GetWorld())
    {
        STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
        if (!STUGameInstance) return;
        const auto StartedLevel = STUGameInstance->GetStartupLevel();
    }
}

void ASTUGameModeBase::StartPlay() 
{
    Super::StartPlay();
    if (!STUGameInstance) return;

    // берем настройки из инстанса
    RoundsNum = STUGameInstance->GetRounds();
    PlayersNum = STUGameInstance->GetPlayersNum();
    RoundTime = STUGameInstance->GetRoundTime();
    PlayersName = STUGameInstance->GetPlayersName();
    GameType = STUGameInstance->GetStartupLevel().GameType;
    PlayerTeamColor = STUGameInstance->GetPlayerTeamColor();
    EnemyTeamColor = STUGameInstance->GetEnemyTeamColor();

    //CreateSpawners(); // создаем спавнеры
    SpawnBots(); // создаем ботов
    CreateTeamsInfo(); // распределяем ботов по командам

    StartRound(); // старт первого раунда
    SetMatchState(ESTUMatchState::InProgress); // стейт выставляется в прогресс

    defaultHUD = Cast<ASTUGameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD()); //ссылка на хад
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

//void ASTUGameModeBase::CreateSpawners() // создание спавнеров
//{
//    TArray<AActor*> Spawners;
//    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABotSpawner::StaticClass(), Spawners); //ищем все спавнеры на сцене и добавляем в массив
//    if (Spawners.Num() == 2)
//    {
//        FirstTeam = Cast<ABotSpawner>(Spawners[0]);
//        SecondTeam = Cast<ABotSpawner>(Spawners[1]);
//        if (FirstTeam->GetTeamID() == 2)
//            Swap(FirstTeam, SecondTeam);
//    }
//}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;

    for (int32 i = 0; i < PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        {
            RestartPlayer(STUAIController);
        }
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

// оверрайд
void ASTUGameModeBase::CreateTeamsInfo() // распределяем ботов по командам
{
}

// оверрайд
FSTUColors ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    return FSTUColors{ FLinearColor::Red, TEXT("Red") };
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

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController, bool HeadShot)
{
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    VictimPlayerState->AddDeath();
    StartRespawn(VictimController);
    if (KillerPlayerState)
    {
        if (KillerPlayerState->GetTeamID() == VictimPlayerState->GetTeamID())
        {
            KillerPlayerState->AddKillTeammate();
            if (KillerPlayerState->GetTeamID() == 1)
                PlayerTeamPoints--;
            else
                EnemiesTeamPoints--;
        }
        else
        {
            KillerPlayerState->AddKill();
            if (KillerPlayerState->GetTeamID() == 1)
                PlayerTeamPoints++;
            else
                EnemiesTeamPoints++;
        }
        defaultHUD->addKill(KillerPlayerState->GetPlayerName(), KillerPlayerState->GetTeamID(), VictimPlayerState->GetPlayerName(), VictimPlayerState->GetTeamID(), HeadShot);
    }
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAviable = RoundCountDown > MinRoundTimeForRespawn + RespawnTime;
    if (!RespawnAviable) return;
    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
    if (!RespawnComponent) return;
    RespawnComponent->Respawn(RespawnTime); // RespawnRequest
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
    GameResult = SetGameResult();
    SetMatchState(ESTUMatchState::GameOver);
}

ESTUGameResult ASTUGameModeBase::SetGameResult()
{
    if (PlayerTeamPoints > EnemiesTeamPoints)
        return ESTUGameResult::Victory;
    else if (PlayerTeamPoints < EnemiesTeamPoints)
        return ESTUGameResult::Defeat;
    else
        return ESTUGameResult::Draw;
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