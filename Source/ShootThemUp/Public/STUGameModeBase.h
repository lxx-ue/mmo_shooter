// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;
class ASTUGameHUD;
class ABotSpawner;
class USTUGameInstance;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    ASTUGameModeBase();
    FOnMatchStateChangedSignature OnMathcStateChanged;
    virtual void StartPlay() override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
    void Killed(AController* KillerController, AController* VictimController, bool HeadShot);
    int32 GetRoundsNum() const { return RoundsNum; }
    int32 GetCurrentRoundNum() const { return CurrentRound; }
    int32 GetRoundSecondsRemaining() const { return RoundCountDown; }
    int32 GetPlayerTeamPoints() const { return PlayerTeamPoints; }
    int32 GetEnemiesTeamPoints() const { return EnemiesTeamPoints; }
    void RespawnRequest(AController* Controller);
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;
    ESTUGameResult GetGameResult() { return GameResult; }
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
        TSubclassOf<AAIController> AIControllerClass;
    UPROPERTY(EditDefaultsOnly, Category = "Game")
        TSubclassOf<APawn> AIPawnClass;
    int32 PlayersNum;
    int32 RoundsNum;
    int32 RoundTime;
    FString PlayersName;
    FLinearColor PlayerTeamColor = FLinearColor::Blue;
    FLinearColor EnemyTeamColor = FLinearColor::Red;
    int32 RespawnTime = 5;

    ABotSpawner* FirstTeam;
    ABotSpawner* SecondTeam;

    USTUGameInstance* STUGameInstance;
    ESTUGameType GameType;
    ESTUGameResult GameResult;

    int32 PlayerTeamPoints = 0;
    int32 EnemiesTeamPoints = 0;

private:
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;
    int32 BotSpawnMode = 1;

    void SpawnBots();
    void StartRound();
    void GameTimerUpdate();
    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);
    void CreateTeamsInfo();
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;
    void SetPlayerColor(AController* Controller);
    void StartRespawn(AController* Controller);
    void GameOver();
    void SetMatchState(ESTUMatchState State);
    void StopAllFire();
    void CreateSpawners();
    ESTUGameResult SetGameResult();
    FString GetRandomBotName() const;
    ASTUGameHUD* defaultHUD;

    TArray<FString> botnames = {
        TEXT("James"),
        TEXT("Robert"),
        TEXT("John"),
        TEXT("Michael"),
        TEXT("David"),
        TEXT("William"), 
        TEXT("Richard"),
        TEXT("Joseph"),
        TEXT("Thomas"),
        TEXT("Charles"),
        TEXT("Christopher"),
        TEXT("Daniel"),
        TEXT("Matthew"),
        TEXT("Anthony"),
        TEXT("Mark")
    };
};
