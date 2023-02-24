// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUGameModeBase.h"
#include "STUGameModeTeamDeathMatch.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameModeTeamDeathMatch : public ASTUGameModeBase
{
	GENERATED_BODY()
	
private:
	virtual void CreateTeamsInfo() override;
	virtual FLinearColor DetermineColorByTeamID(int32 TeamID) const override;
};
