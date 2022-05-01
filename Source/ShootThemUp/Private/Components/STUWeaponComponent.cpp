
#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i items"), WeaponNum);
    //поиск анимаций
    InitAnimations();
    //номер текущего оружия
    CurrentWeaponIndex = 0;
    //спавн оружия
    SpawnWeapons();
    //надеть оружие
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) //конец игры
{
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); //отсоединяем оружия от перса
        Weapon->Destroy(); //удаляем оружие со сцены
    }
    Weapons.Empty(); //очищаем массим
    Super::EndPlay(EndPlayReason); //наследуем базовую логику
}

void USTUWeaponComponent::SpawnWeapons() //спавн оружий
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Character) return; //мир не создан - выходим

    for (auto OneWeaponData : WeaponData)
    {
        //спавн оружия
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue; //оружие не заспавнилось - выходим

        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnClipEmpty); //делегируем функции пустой обоймы
        Weapon->SetOwner(Character); //задаем родителя
        Weapons.Add(Weapon); //добавляем оружие в массив
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName); //аттачим 
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) return;
    //аттачим оружие к игроку
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) //взять оружие
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) //если оружие есть
    {
        //UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }
    ACharacter* Character = Cast<ACharacter>(GetOwner()); //ссылка на персонажа
    if (!Character) return; 

    if (CurrentWeapon)
    {
        CurrentWeapon->Zoom(false);
        CurrentWeapon->StopFire(); //прекращаем стрельбу
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName); //аттачим оружие к персонажу
    }

    CurrentWeapon = Weapons[WeaponIndex];
    //CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass();});
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire() 
{
    //вызываем функцию начала стрельбы из самого оружия
    if (!CanFire()) return;
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    //вызываем функцию остановки стрельбы из самого оружия
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon() //сменить оружие
{
    if (!CanEquip()) return; //если можешь
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num(); //меняем номер текущего оружия
    EquipWeapon(CurrentWeaponIndex); //переключаем оружие по номеру
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) //старт анимации
{
    ACharacter* Character = Cast<ACharacter>(GetOwner()); //сылка на персонажа
    if (!Character) return; //если ее нет - выходим
    Character->PlayAnimMontage(Animation); //играем анимацию в классе персонажа
}

void USTUWeaponComponent::InitAnimations() 
{
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage); //ищем нотифай нужного класса в нужной анимации
    if (EquipFinishedNotify) //нотифай найден
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished); //делегируем функцию оружие уже сменилось 
    }
    else //нотифай не найден
    {
        //UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
        checkNoEntry();
    }
    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify) //нотифай не найден
        {
            //UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
            checkNoEntry();
        }
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished); //делегируем функцию оружие уже перезарядилось 
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp) ////закончил смену оружия
{
    ACharacter* Character = Cast<ACharacter>(GetOwner()); //получаешь ссылку на персонажа
    if (!Character || Character->GetMesh() != MeshComp) return; //если персонажа нет или меш не совпадает с нашим

    EquipAnimInProgress = false; //закончил смену оружия
    //UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
 }

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp) //когда закончил перезарядку
 {
     ACharacter* Character = Cast<ACharacter>(GetOwner()); //получаешь ссылку на персонажа
     if (!Character || Character->GetMesh() != MeshComp) return; //если персонажа нет или меш не совпадает с нашим

     ReloadAnimInProgress = false; //закончил перезарядку
     //UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
 }

bool USTUWeaponComponent::CanFire() const  //может ли стрелять
{
    // текущее оружие заспавнилось, не меняешь оружие и не перезаряжаешься
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const //может ли сменить оружие
{
    //не меняешь оружие и не перезаряжаешься
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const //может ли перезарядиться
{
    //текущее оружие заспавнилось, не переключаешь оружие, не перезаряжаешься и может ли оружие быть перезаряжено 
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::Reload() //когда обойма пуста
{
    ChangeClip();  //перезарядиться
}

void USTUWeaponComponent::OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon) //когда обойма пуста
{
    if (!AmmoEmptyWeapon) return;
    if (CurrentWeapon == AmmoEmptyWeapon)
    {
        ChangeClip(); // перезарядиться
    }
    else
    {
        for (const auto Weapon : Weapons)
        {
            if (Weapon == AmmoEmptyWeapon)
            {
                Weapon->ChangeClip(); //при оружия патронов перезяряжаем пустое оружие
            }
        }
    }
}

void USTUWeaponComponent::ChangeClip() //перезарядиться
{
    if (!CanReload()) return; //может ли..?
    CurrentWeapon->StopFire(); //остановить стрельбу текущего оружия
    //начать анимацию перезарядки
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
    CurrentWeapon->ChangeClip(); //сменить обойму текущего оружия
}

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const //возварщает иконки
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const //возвращает патроны
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount) //попытка подобрать патроны
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType)) //если тип оружия совпадает с нашим арсеналом
        {
            return Weapon->TryToAddAmmo(ClipsAmount); //пытаемся подобрать
        }
    }
    return false;
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType) 
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType)) //если тип оружия совпадает с нашим арсеналом
        {
            return !Weapon->IsAmmoFull();
        }
    }
    return false;
}

void USTUWeaponComponent::Zoom(bool Enabled)
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Zoom(Enabled);
    }
}