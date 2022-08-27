// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/STULevelItemWidget.h"
#include "Sound/SoundCue.h"
#include "Components/EditableTextBox.h"
#include "Saves/STUSaveGame.h"

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
	//if (etb_PlayerName)
	//{
		etb_PlayerName->OnTextCommitted.AddDynamic(this, &USTUMenuWidget::OnNameChanged);
	//}

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

void USTUMenuWidget::OnNameChanged(const FText& InText, ETextCommit::Type InCommitType) // ñîõð èìåíè èãðîêà
{
	//auto nam = etb_PlayerName->GetText();
	FString SomeString = InText.ToString();
	UE_LOG(LogTemp, Log, TEXT("SomeString: %s"), *SomeString);
	STUGameInstance->SetPlayersName(SomeString);

	if (USTUSaveGame* SaveGameInstance = Cast<USTUSaveGame>(UGameplayStatics::CreateSaveGameObject(USTUSaveGame::StaticClass())))
	{
		SaveGameInstance->PlayersName = SomeString;
		// ÍÀÄÎ ÁÓÄÅÒ ÏÎÄÃÐÓÆÀÒÜ ÑÒÀÒÛ ÈÇ STUGameInstance È ÇÀÍÎÑÈÒÜ Â SaveGameInstance

		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "stats", 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("SAVED: %s"), *SaveGameInstance->PlayersName);
		}
	}
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
	if (!GetWorld()) return nullptr;
	return GetWorld()->GetGameInstance<USTUGameInstance>();
}
