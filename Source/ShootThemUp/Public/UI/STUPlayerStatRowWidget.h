// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlateBasics.h"
#include "STUPlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()

	FLinearColor PlayerColor = FLinearColor(0.85f, 0.65f, 0.1f, 1.0f);


public:

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetPlayerName(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetKills(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetDeaths(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetPlayerIndicatorVisibility(bool Visible);

	int32 kills, deaths;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillsTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathsTextBlock;
};
