// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "STUCoreTypes.h"
#include "STUSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayersName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 PlayersKills;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 PlayersDeaths;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FSTUColors PlayerTeamColor;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FSTUColors EnemyTeamColor;
};
