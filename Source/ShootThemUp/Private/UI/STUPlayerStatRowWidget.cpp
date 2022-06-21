// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/STUPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (!PlayerNameTextBlock) return;
	PlayerNameTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetKills(const FText& Text)
{
	if (!KillsTextBlock) return;
	KillsTextBlock->SetText(Text);
	kills = FCString::Atoi(*Text.ToString());
}

void USTUPlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if (!DeathsTextBlock) return;
	DeathsTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
	if (Visible)
	{
		PlayerNameTextBlock->SetColorAndOpacity(PlayerColor);
		KillsTextBlock->SetColorAndOpacity(PlayerColor);
		DeathsTextBlock->SetColorAndOpacity(PlayerColor);
	}
}