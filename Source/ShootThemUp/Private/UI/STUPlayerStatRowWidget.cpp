// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/STUPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (!PlayerNameTextBlock) return;
	PlayerNameTextBlock->SetText(Text);
	//if (Text.ToString() == "Player") PlayerNameTextBlock->SetFont(fontinfo);
}

void USTUPlayerStatRowWidget::SetKills(const FText& Text)
{
	if (!KillsTextBlock) return;
	KillsTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if (!DeathsTextBlock) return;
	DeathsTextBlock->SetText(Text);
}

//void USTUPlayerStatRowWidget::SetTeam(const FText& Text)
//{
//	if (!TeamTextBlock) return;
//	TeamTextBlock->SetText(Text);
//}

void USTUPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
	//if (!PlayerIndicatorImage) return;
	//PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	//FLinearColor color = FLinearColor(0.7f, 0.7f, 0.7f);
	if (Visible)
	{
		//PlayerNameTextBlock->SetText(FText::FromString("Damn"));
		PlayerNameTextBlock->SetColorAndOpacity(PlayerColor);
		KillsTextBlock->SetColorAndOpacity(PlayerColor);
		DeathsTextBlock->SetColorAndOpacity(PlayerColor);
		//PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void USTUPlayerStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
	if (!PlayerIndicatorImage) return;
	PlayerNameTextBlock->SetColorAndOpacity(Color);
}