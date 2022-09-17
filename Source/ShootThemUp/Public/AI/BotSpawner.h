// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BotSpawner.generated.h"

class USceneComponent;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ABotSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABotSpawner();
	int32 GetTeamID() { return TeamID; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
		TArray<FVector> Points;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team", meta = (ClampMin = "1.0", ClampMax = "2.0"))
		int32 TeamID = 1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
