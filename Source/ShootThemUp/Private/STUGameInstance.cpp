// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameInstance.h"
#include "Sound/STUSoundFuncLib.h"

void USTUGameInstance::ToggleVolume()
{
	USTUSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}
