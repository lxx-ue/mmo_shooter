// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/STUPauseWidget.h"
#include "Components/Button.h"
#include "Gameframework/GameModeBase.h"


void USTUPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
	}
}

void USTUPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	GetWorld()->GetAuthGameMode()->ClearPause();
}