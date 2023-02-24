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
class UEditableTextBox;
class UImage;
class UCanvasPanel;

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
	UButton* b_players_up;

	UPROPERTY(meta = (BindWidget))
	UButton* b_players_down;

	UPROPERTY(meta = (BindWidget))
	UButton* b_round_time_up;

	UPROPERTY(meta = (BindWidget))
	UButton* b_round_time_down;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* etb_PlayerName;

	UPROPERTY(meta = (BindWidget))
	UImage* img_player_color;

	UPROPERTY(meta = (BindWidget))
	UImage* img_enemy_color;


	UPROPERTY(meta = (BindWidget))
		UButton* b_red_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_blue_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_orange_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_green_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_cyan_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_yellow_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_purple_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_pink_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_black_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_white_color;


	UPROPERTY(meta = (BindWidget))
	UButton* b_label_canvas;

	UPROPERTY(meta = (BindWidget))
	UButton* b_solo_game_canvas;

	UPROPERTY(meta = (BindWidget))
	UButton* b_authors_canvas;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_menu;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_player_info;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_content;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_blank;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_color_selecter;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_solo_game;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_authors;

	UPROPERTY(meta = (BindWidget))
		UButton* b_change_player_color;

	UPROPERTY(meta = (BindWidget))
		UButton* b_change_enemy_color;
	
	UPROPERTY(meta = (BindWidget))
		UButton* b_save_color;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* HideAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* StartGameSound;

	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	bool isPlayerColorSelecting;

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
	UFUNCTION()
	void OnPlayersUp();
	UFUNCTION()
	void OnPlayersDown();
	UFUNCTION()
	void OnRoundTimeUp();
	UFUNCTION()
	void OnRoundTimeDown();
	UFUNCTION()
	void OnNameChanged(const FText& InText, ETextCommit::Type InCommitType);


	UFUNCTION()
	void OnLabelCanvasButtonSelected();
	UFUNCTION()
	void OnSoloGameCanvasButtonSelected();
	UFUNCTION()
	void OnAuthorsCanvasButtonSelected();
	UFUNCTION()
		void OnChangePlayerColor();
	UFUNCTION()
		void OnChangeEnemyColor();
	UFUNCTION()
		void OnColorSelected();

	UFUNCTION()
		void OnRedColorSelected();
	UFUNCTION()
		void OnBlueColorSelected();
	UFUNCTION()
		void OnOrangeColorSelected();
	UFUNCTION()
		void OnGreenColorSelected();
	UFUNCTION()
		void OnCyanColorSelected();
	UFUNCTION()
		void OnYellowColorSelected();
	UFUNCTION()
		void OnPurpleColorSelected();
	UFUNCTION()
		void OnPinkColorSelected();
	UFUNCTION()
		void OnBlackColorSelected();
	UFUNCTION()
		void OnWhiteColorSelected();

	void SwitchCanvas(int32 CanvasNum);
	void InitLevelItems();
	void OnLevelSelected(const FLevelData& Data);
	USTUGameInstance* GetSTUGameInstance() const;
	USTUGameInstance* STUGameInstance;

	void SetSceneCharacterColor();

	FLinearColor SelectedColor;
};
