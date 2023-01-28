// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    void SetTeamID(int32 ID) { TeamID = ID; }
	void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; }
	void AddKill() { ++KillsNum; }
	void AddKillTeammate() { --KillsNum; }
	void AddDeath() { ++DeathsNum; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	FLinearColor GetTeamColor() const { return TeamColor; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetKillsNum() const { return KillsNum; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
    int32 GetDeathsNum() const { return DeathsNum; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetTeamID() const { return TeamID; }

private: 
	int32 TeamID;
    FLinearColor TeamColor;

	int32 KillsNum = 0;
    int32 DeathsNum = 0;
};
