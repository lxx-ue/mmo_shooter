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
	void SetRounds(int32 n) { Rounds = FMath::Clamp(n, 1, 10); }

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetRounds() const { return Rounds; }

	void ToggleVolume();
	int32 Rounds = 1;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
		TArray<FLevelData> LevelsData;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
		FName MenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundClass* MasterSoundClass;

private:
	FLevelData StartupLevel;

};
