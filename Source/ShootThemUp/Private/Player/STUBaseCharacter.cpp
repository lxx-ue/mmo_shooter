// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/STUBaseCharacter.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;    

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    //�������� ������� �����������
    check(HealthComponent);
    check(GetCharacterMovement());
    check(GetMesh());

    //������ ��������� ��
    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
    //�������������
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthData)
{
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    //���� �������� ���� 
    if (GetVelocity().IsZero()) return 0.0f; 
    //���������� ������� ������� ��������
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    //��������� ������������ ����� ������� �������� � ��������, ����������� �������� ���� ����� ���������
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    //���������� ������������� ������
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    //��������� � ������� 
    const auto Degress = FMath::RadiansToDegrees(AngleBetween);
    //���������� ��������
    return CrossProduct.IsZero() ? Degress : Degress * FMath::Sign(CrossProduct.Z);
}

bool ASTUBaseCharacter::IsRunnig() const
{
    return false;
}

void ASTUBaseCharacter::OnDeath() {
    //UE_LOG(LogBaseCharacter, Display, TEXT("%s death"), *GetName());
    //�������� ������
    PlayAnimMontage(DeathAnimMontage);
    //���������� ��������
    GetCharacterMovement()->DisableMovement();
    //����� 5 ������ ������� ������ ������
    SetLifeSpan(5.0f);
    //��������� �������� �������
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);
    //������
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit) 
{
    //������������ ������ ������ (�������� ������ ����)
    const auto FallVelocityZ = -GetVelocity().Z;
    //UE_LOG(LogBaseCharacter, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X) return; //���� �� ������� ������ - �������

    //������������ ���� � �������� ��������� 
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    //UE_LOG(LogBaseCharacter, Display, TEXT("Damage: %f"), FinalDamage);
    //������� ����
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if(!MaterialInst) return;

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}

void ASTUBaseCharacter::TurnOff()
{
    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);
    Super::TurnOff();
}
void ASTUBaseCharacter::Reset()
{
    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);
    Super::Reset();
}