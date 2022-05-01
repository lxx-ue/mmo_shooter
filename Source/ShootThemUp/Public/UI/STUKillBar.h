// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUKillBar.generated.h"

class UTextBlock;

UCLASS()
class SHOOTTHEMUP_API USTUKillBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayersName(FText killer, FText victim);

protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* killername;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* victimname;

	
};
