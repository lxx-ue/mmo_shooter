// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUDevDamageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* SceneComponent;

	//радиус сферы
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius = 300.0f;
	
	//цвет сферы
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor SphereColor = FColor::Red;
	
	//кол-во урона
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 10.0f;

	//полный урон
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool DoFullDamage = false;

	//тип урона
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UDamageType> DamageType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
