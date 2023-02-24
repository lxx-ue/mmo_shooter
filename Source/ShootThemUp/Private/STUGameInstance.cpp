// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameInstance.h"
#include "Sound/STUSoundFuncLib.h"
#include "Saves/STUSaveGame.h"
#include "Kismet/GameplayStatics.h"

void USTUGameInstance::ToggleVolume()
{
	USTUSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}

void USTUGameInstance::SaveStats()
{
	if (USTUSaveGame* SaveGameInstance = Cast<USTUSaveGame>(UGameplayStatics::CreateSaveGameObject(USTUSaveGame::StaticClass())))
	{
		SaveGameInstance->PlayersName = PlayersName;
		SaveGameInstance->PlayersKills = PlayersKills;
		SaveGameInstance->PlayersDeaths = PlayersDeaths;
		SaveGameInstance->PlayerTeamColor = PlayerTeamColor;
		SaveGameInstance->EnemyTeamColor = EnemyTeamColor;

		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "stats", 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("SAVED: %s"), *SaveGameInstance->PlayersName);
		}
	}
}