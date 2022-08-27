// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;
class ASTUGameHUD;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    ASTUGameModeBase();
    FOnMatchStateChangedSignature OnMathcStateChanged;
    virtual void StartPlay() override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
    void Killed(AController* KillerController, AController* VictimController);
    int32 GetRoundsNum() const { return RoundsNum; }
    int32 GetCurrentRoundNum() const { return CurrentRound; }
    int32 GetRoundSecondsRemaining() const { return RoundCountDown; }
    void RespawnRequest(AController* Controller);
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;
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

private:
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

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
    FString GetRandomBotName() const;

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
