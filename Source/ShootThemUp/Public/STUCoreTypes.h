#pragma once

#include "STUCoreTypes.generated.h"

//����� �������� �������� �������� ��������� ������������ � ������ ������

//weapon
class ASTUBaseWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASTUBaseWeapon*); //������� �������� ������

USTRUCT(BlueprintType)
struct FAmmoData //��������� ��������
{
    GENERATED_USTRUCT_BODY()

    //���������� �������� � ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    //���������� ����� 
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips;

    //���������� �������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponData //��������� ������
{
    GENERATED_USTRUCT_BODY()

    //����� ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    //�������� �����������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FWeaponUIData 
{
    GENERATED_USTRUCT_BODY()

    //������ ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;

    //������ ������� ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrossHairIcon;

};


//health
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature); //������� ����������� ������
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float); //������� ��������� ��


//VFX

class UNiagaraSystem;
class USoundCue;

USTRUCT(BlueprintType)
struct FDecalData 
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UMaterialInterface* Material;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FVector Size = FVector(10.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float LifeTime = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float FadeOutTime = 0.7f;
};

USTRUCT(BlueprintType)
struct FImpactData 
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* NiagaraEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FDecalData DecalData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    USoundCue* Sound;

};

USTRUCT(BlueprintType)
struct FGameData 
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "36"))
    int32 PlayersNum = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
    int32 RoundsNum = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
    int32 RoundTime = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor DefaultTeamColor = FLinearColor::Red;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<FLinearColor> TeamColors;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "0", ClampMax = "20"))
    int32 RespawnTime = 5;
};

UENUM(BlueprintType)
enum class ESTUMatchState : uint8
{
    WaitingToStart = 0,
    InProgress,
    Pause,
    GameOver
};

UENUM(BlueprintType)
enum class ESTUGameType : uint8
{
    TeamDeathMatch = 0,
    SingleDeathMatch,
    FlagCapture,
    MountainKing
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, ESTUMatchState);

USTRUCT(BlueprintType)
struct FLevelData //��������� ������
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
    FName LevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
    FName LevelDisplayName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
    UTexture2D* LevelThumb;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        ESTUGameType GameType;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);

UENUM(BlueprintType)
enum class ESTUGameResult : uint8
{
    Victory = 0,
    Defeat,
    Draw
};

USTRUCT(BlueprintType)
struct FSTUColors
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Color")
    FLinearColor color;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Color")
    FString name;
};
