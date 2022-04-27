// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTUGameInstance;
class USTULevelItemWidget;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* b_rounds_up;

	UPROPERTY(meta = (BindWidget))
	UButton* b_rounds_down;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* HideAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* StartGameSound;

	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	UPROPERTY()
		TArray<USTULevelItemWidget*> LevelItemWidgets;

	UFUNCTION()
	void OnStartGame();
	UFUNCTION()
	void OnQuitGame();
	UFUNCTION()
	void OnRoundsUp();
	UFUNCTION()
	void OnRoundsDown();

	void InitLevelItems();
	void OnLevelSelected(const FLevelData& Data);
	USTUGameInstance* GetSTUGameInstance() const;
	USTUGameInstance* STUGameInstance;
};
