// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"

class USoundClass;

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	FLevelData GetStartupLevel() const { return StartupLevel; }
	void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

	TArray<FLevelData> GetLevelsData() const { return LevelsData; }
	FName GetMenuLevelName() const { return MenuLevelName; }

	void SetRounds(int32 n) { RoundsNum = FMath::Clamp(n, 1, 10); }
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetRounds() const { return RoundsNum; }

	void SetPlayersNum(int32 n) { PlayersNum = FMath::Clamp(n, 2, 10); }
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetPlayersNum() const { return PlayersNum; }

	void SetRoundTime(int32 n) { RoundTime = FMath::Clamp(n, 30, 300); }
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetRoundTime() const { return RoundTime; }

	void SetPlayersName(FString n) { PlayersName = n; }
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	FString GetPlayersName() const { return PlayersName; }

	void ToggleVolume();

	//gamedata


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
		TArray<FLevelData> LevelsData;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
		FName MenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundClass* MasterSoundClass;

private:
	FLevelData StartupLevel;
	int32 RoundsNum = 1;
	int32 PlayersNum = 2;
	int32 RoundTime = 30;
	FString PlayersName = "Player";
};
