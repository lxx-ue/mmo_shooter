// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "STUGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"
#include <ShootThemUp/Public/Player/STUPlayerController.h>

//#include "Camera/CameraShake.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    //Стартовое хп не ноль
    check(MaxHealth > 0);
	//на старте здоровье максимальное
    SetHealth(MaxHealth);
    //подписка на делегат события получения урона
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
    }
}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
    if (InstigatedBy)
    {
        const auto FinalDamage = Damage * GetPointDamagedModifier(DamagedActor, BoneName);
        ApplyDamage(FinalDamage, InstigatedBy);
        UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, final damage: %f, bone: %s"), Damage, FinalDamage, *BoneName.ToString());
    }
}

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (InstigatedBy && !IsDead())
    {
        ApplyDamage(Damage, InstigatedBy);
        UE_LOG(LogHealthComponent, Display, TEXT("On radial damage: %f"), Damage);  
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (!InstigatedBy && !IsDead())
    {
        ApplyDamage(Damage, InstigatedBy);
        UE_LOG(LogHealthComponent, Display, TEXT("On any damage: %f"), Damage);
    }
}

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld())
        return; //если урон отрицательный или мы уже мертвы выходим из функции

    const auto Player = Cast<APawn>(GetOwner());
    const auto VictimController = Player ? Player->Controller : nullptr;
    VictimController->IsPlayerController();
    const auto PlayerController = Cast<ASTUPlayerController>(VictimController);
    if (PlayerController && PlayerController->bGodMode) return;

    //установка хп исходя из урона
    SetHealth(Health - Damage);
    //очищаем таймер восстановления хп
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    if (IsDead())
    {
        //смерть
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        //автовосстановление хп
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
    PlayCameraShake(); //вызов тряски
    ReportDamageEvent(Damage, InstigatedBy);
}

void USTUHealthComponent::HealUpdate() 
{
    //восстановлние хп
    SetHealth(Health + HealModifier);
    if (IsHealthFull() && GetWorld())
    {
        //если хп на максимуме останаливаем таймер
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}
void USTUHealthComponent::SetHealth(float NewHealth) 
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;
    //установка нового значения хп
    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull()) return false; //если мертв или фулл хп
    SetHealth(Health + HealthAmount); //иначе восполняем хп
    return true;
}

bool USTUHealthComponent::IsHealthFull() const //полное ли хп
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::PlayCameraShake() 
{
    if (IsDead()) return;
    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;
    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;
    Controller->PlayerCameraManager->StartCameraShake(CameraShake); //тряска камеры
}

void USTUHealthComponent::Killed(AController* KillerController)
{
    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto Player = Cast<APawn>(GetOwner());
    const auto VictimController = Player ? Player->Controller : nullptr;
    GameMode->Killed(KillerController, VictimController);
}

float USTUHealthComponent::GetPointDamagedModifier(AActor* DamagedActor, const FName& BoneName)
{
    const auto Character = Cast<ACharacter>(DamagedActor);
    if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName)) return 1.0f;

    const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!PhysMaterial || !DamagedModifiers.Contains(PhysMaterial)) return 1.0f;
    return DamagedModifiers[PhysMaterial];
}

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
    if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;
    UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
}