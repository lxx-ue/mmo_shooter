// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BotSpawner.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABotSpawner::ABotSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABotSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABotSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

