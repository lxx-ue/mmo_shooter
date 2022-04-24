// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All)

// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);
}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
    check(CollisionComponent);

    GenerateRoatationYaw();
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f)); //вращение пикапа
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)  //оверлав пикапа
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor); //если это наш игрок
    if (GivePickupTo(Pawn)) //можно ли взять пикап
    {
        PickupWasTaken(); //подбираем пикап
    }
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn) 
{
    return false; // в базовом классе не подобрать, смотри перегрузки дочерних классов
}

void ASTUBasePickup::PickupWasTaken() //подобрали
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //отключаем коллизию
    if (GetRootComponent())
    {
        GetRootComponent()->SetVisibility(false, true); //и отключаем видимость
    }
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupTakenSound, GetActorLocation());
}

void ASTUBasePickup::Respawn() //респавн
{
    GenerateRoatationYaw(); //меняем вращение
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); //включаем коллизию
    if (GetRootComponent())
    {
        GetRootComponent()->SetVisibility(true, true); // и включаем видимость
    }
}

void ASTUBasePickup::GenerateRoatationYaw() //генерация вращения
{
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
    RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}

bool ASTUBasePickup::CouldBeTaken() const
{
    return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}