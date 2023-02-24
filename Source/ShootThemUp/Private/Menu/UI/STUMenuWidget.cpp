// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/STULevelItemWidget.h"
#include "Sound/SoundCue.h"
#include "Components/EditableTextBox.h"
#include "Saves/STUSaveGame.h"
#include "Player/STUBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
	}
	if (b_rounds_up)
	{
		b_rounds_up->OnClicked.AddDynamic(this, &USTUMenuWidget::OnRoundsUp);
	}
	if (b_rounds_down)
	{
		b_rounds_down->OnClicked.AddDynamic(this, &USTUMenuWidget::OnRoundsDown);
	}
	if (b_players_up)
	{
		b_players_up->OnClicked.AddDynamic(this, &USTUMenuWidget::OnPlayersUp);
	}
	if (b_players_down)
	{
		b_players_down->OnClicked.AddDynamic(this, &USTUMenuWidget::OnPlayersDown);
	}
	if (b_round_time_up)
	{
		b_round_time_up->OnClicked.AddDynamic(this, &USTUMenuWidget::OnRoundTimeUp);
	}
	if (b_round_time_down)
	{
		b_round_time_down->OnClicked.AddDynamic(this, &USTUMenuWidget::OnRoundTimeDown);
	}
	if (etb_PlayerName)
	{
		etb_PlayerName->OnTextCommitted.AddDynamic(this, &USTUMenuWidget::OnNameChanged);
	}
	if (b_label_canvas)
	{
		b_label_canvas->OnClicked.AddDynamic(this, &USTUMenuWidget::OnLabelCanvasButtonSelected);
	}
	if (b_solo_game_canvas)
	{
		b_solo_game_canvas->OnClicked.AddDynamic(this, &USTUMenuWidget::OnSoloGameCanvasButtonSelected);
	}
	if (b_authors_canvas)
	{
		b_authors_canvas->OnClicked.AddDynamic(this, &USTUMenuWidget::OnAuthorsCanvasButtonSelected);
	}
	if (b_change_player_color)
	{
		b_change_player_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnChangePlayerColor);
	}
	if (b_change_enemy_color)
	{
		b_change_enemy_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnChangeEnemyColor);
	}
	if (b_save_color)
	{
		b_save_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnColorSelected);
	}
	if (b_red_color)
	{
		b_red_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnRedColorSelected);
	}
	if (b_blue_color)
	{
		b_blue_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnBlueColorSelected);
	}
	if (b_orange_color)
	{
		b_orange_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnOrangeColorSelected);
	}
	if (b_green_color)
	{
		b_green_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnGreenColorSelected);
	}
	if (b_cyan_color)
	{
		b_cyan_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnCyanColorSelected);
	}
	if (b_yellow_color)
	{
		b_yellow_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnYellowColorSelected);
	}
	if (b_purple_color)
	{
		b_purple_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnPurpleColorSelected);
	}
	if (b_pink_color)
	{
		b_pink_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnPinkColorSelected);
	}
	if (b_black_color)
	{
		b_black_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnBlackColorSelected);
	}
	if (b_white_color)
	{
		b_white_color->OnClicked.AddDynamic(this, &USTUMenuWidget::OnWhiteColorSelected);
	}
	InitLevelItems();
}

void USTUMenuWidget::InitLevelItems()
{
	STUGameInstance = GetSTUGameInstance();
	if (!STUGameInstance) return;
	checkf(STUGameInstance->GetLevelsData().Num()!= 0, TEXT("Levels data must not be empty"));

	if (!LevelItemBox) return;
	LevelItemBox->ClearChildren();

	for (FLevelData LevelData : STUGameInstance->GetLevelsData())
	{
		const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if (!LevelItemWidget) continue;
		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);

		LevelItemBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}

	if (STUGameInstance->GetStartupLevel().LevelName.IsNone())
		OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
	else
		OnLevelSelected(STUGameInstance->GetStartupLevel());

	if (USTUSaveGame* LoadedGame = Cast<USTUSaveGame>(UGameplayStatics::LoadGameFromSlot("stats", 0)))
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		UE_LOG(LogTemp, Warning, TEXT("LOADED: %s"), *LoadedGame->PlayersName);
		// etb_PlayerName;
		STUGameInstance->SetPlayersName(LoadedGame->PlayersName);
		STUGameInstance->SetPlayersKills(LoadedGame->PlayersKills);
		STUGameInstance->SetPlayersDeaths(LoadedGame->PlayersDeaths);
		img_player_color->SetColorAndOpacity(LoadedGame->PlayerTeamColor);
		STUGameInstance->SetPlayerTeamColor(LoadedGame->PlayerTeamColor);
		img_enemy_color->SetColorAndOpacity(LoadedGame->EnemyTeamColor);
		STUGameInstance->SetEnemyTeamColor(LoadedGame->EnemyTeamColor);
	}
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	if (!STUGameInstance) return;

	STUGameInstance->SetStartupLevel(Data);

	for (auto LevelItemWidget : LevelItemWidgets)
	{
		if (LevelItemWidget)
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(IsSelected);
		}
	}
}

void USTUMenuWidget::OnStartGame()
{
	STUGameInstance->SetPlayerTeamColor(img_player_color->ColorAndOpacity);
	STUGameInstance->SetEnemyTeamColor(img_enemy_color->ColorAndOpacity);
	PlayAnimation(HideAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation != HideAnimation) return;
	if (!STUGameInstance) return;

	UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
}

void USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTUMenuWidget::OnRoundsUp()
{
	int32 tmp = STUGameInstance->GetRounds() + 1;
	STUGameInstance->SetRounds(tmp);
}

void USTUMenuWidget::OnRoundsDown()
{
	int32 tmp = STUGameInstance->GetRounds() - 1;
	STUGameInstance->SetRounds(tmp);
}

void USTUMenuWidget::OnPlayersUp()
{
	int32 tmp = STUGameInstance->GetPlayersNum() + 2;
	STUGameInstance->SetPlayersNum(tmp);
}

void USTUMenuWidget::OnPlayersDown()
{
	int32 tmp = STUGameInstance->GetPlayersNum() - 2;
	STUGameInstance->SetPlayersNum(tmp);
}

void USTUMenuWidget::OnRoundTimeUp()
{
	int32 tmp = STUGameInstance->GetRoundTime() + 30;
	STUGameInstance->SetRoundTime(tmp);
}

void USTUMenuWidget::OnRoundTimeDown()
{
	int32 tmp = STUGameInstance->GetRoundTime() - 30;
	STUGameInstance->SetRoundTime(tmp);
}

void USTUMenuWidget::OnNameChanged(const FText& InText, ETextCommit::Type InCommitType) // сохр имени игрока
{
	//auto nam = etb_PlayerName->GetText();
	FString SomeString = InText.ToString();
	UE_LOG(LogTemp, Log, TEXT("SomeString: %s"), *SomeString);
	STUGameInstance->SetPlayersName(SomeString);
	STUGameInstance->SaveStats();
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
	if (!GetWorld()) return nullptr;
	return GetWorld()->GetGameInstance<USTUGameInstance>();
}

void USTUMenuWidget::OnLabelCanvasButtonSelected()
{
	SwitchCanvas(0);
}

void USTUMenuWidget::OnSoloGameCanvasButtonSelected()
{
	SwitchCanvas(1);
}

void USTUMenuWidget::OnAuthorsCanvasButtonSelected()
{
	SwitchCanvas(2);
}

void USTUMenuWidget::SwitchCanvas(int32 CanvasNum)
{
	canvas_solo_game->SetVisibility(ESlateVisibility::Hidden);
	canvas_authors->SetVisibility(ESlateVisibility::Hidden);
	canvas_player_info->SetVisibility(ESlateVisibility::Hidden);
	canvas_color_selecter->SetVisibility(ESlateVisibility::Hidden);

	if (CanvasNum)
		canvas_blank->SetVisibility(ESlateVisibility::Visible);
	else
	{
		canvas_blank->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CanvasNum == 1)
	{
		canvas_solo_game->SetVisibility(ESlateVisibility::Visible);
		canvas_player_info->SetVisibility(ESlateVisibility::Visible);
	}

	if (CanvasNum == 2)
	{
		canvas_authors->SetVisibility(ESlateVisibility::Visible);
	}

}

void USTUMenuWidget::OnChangePlayerColor()
{
	isPlayerColorSelecting = true;
	canvas_solo_game->SetVisibility(ESlateVisibility::Hidden);
	canvas_blank->SetVisibility(ESlateVisibility::Hidden);
	canvas_color_selecter->SetVisibility(ESlateVisibility::Visible);
}

void USTUMenuWidget::OnChangeEnemyColor()
{
	isPlayerColorSelecting = false;
	canvas_solo_game->SetVisibility(ESlateVisibility::Hidden);
	canvas_blank->SetVisibility(ESlateVisibility::Hidden);
	canvas_color_selecter->SetVisibility(ESlateVisibility::Visible);
}

void USTUMenuWidget::OnColorSelected()
{
	canvas_solo_game->SetVisibility(ESlateVisibility::Visible);
	canvas_blank->SetVisibility(ESlateVisibility::Visible);
	canvas_color_selecter->SetVisibility(ESlateVisibility::Hidden);
	if (isPlayerColorSelecting)
	{
		img_player_color->SetColorAndOpacity(SelectedColor);
		STUGameInstance->SetPlayerTeamColor(SelectedColor);
		STUGameInstance->SaveStats();
	}
	else
	{
		img_enemy_color->SetColorAndOpacity(SelectedColor);
		STUGameInstance->SetEnemyTeamColor(SelectedColor);
		STUGameInstance->SaveStats();
	}
}

void USTUMenuWidget::OnRedColorSelected()
{
	SelectedColor = b_red_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::OnBlueColorSelected()
{
	SelectedColor = b_blue_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::OnOrangeColorSelected()
{
	SelectedColor = b_orange_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::OnGreenColorSelected()
{
	SelectedColor = b_green_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::OnCyanColorSelected()
{
	SelectedColor = b_cyan_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::OnYellowColorSelected()
{
	SelectedColor = b_yellow_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::OnPurpleColorSelected()
{
	SelectedColor = b_purple_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::OnPinkColorSelected()
{
	SelectedColor = b_pink_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::OnBlackColorSelected()
{
	SelectedColor = b_black_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::OnWhiteColorSelected()
{
	SelectedColor = b_white_color->ColorAndOpacity;
	SetSceneCharacterColor();
}

void USTUMenuWidget::SetSceneCharacterColor()
{
	TArray<AActor*> Characters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTUBaseCharacter::StaticClass(), Characters);
	const auto Character = Cast<ASTUBaseCharacter>(Characters[0]);
	Character->SetPlayerColor(SelectedColor);
}