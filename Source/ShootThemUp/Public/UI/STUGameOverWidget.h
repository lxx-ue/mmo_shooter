// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public USTUBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
		UVerticalBox* PlayerStatBox;
	UPROPERTY(meta = (BindWidget))
		UVerticalBox* PlayerStatBox_1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;
	UPROPERTY(meta = (BindWidget))
		UButton* ResetLevelButton;

	virtual void NativeOnInitialized() override;

private:
	void OnMatchStateChanged(ESTUMatchState State);
	void UpdatePlayersStat();

	UFUNCTION()
	void OnResetLevel();
};
