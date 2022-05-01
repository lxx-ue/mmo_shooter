// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUKillBar.h"
#include "Components/TextBlock.h"

void USTUKillBar::SetPlayersName(FText killer, FText victim)
{
	if (killername)
	{
		killername->SetText(killer);
	}
	if (victimname)
	{
		victimname->SetText(victim);
	}
	//this->RemoveFromParent();
		
}