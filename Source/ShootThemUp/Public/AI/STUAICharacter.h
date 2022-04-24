// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
	
public:
    ASTUAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BLueprintREadWrite, category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BLueprintREadWrite, category = "AI")
		float HealthVisibilityDistance = 1000.0f;

	virtual void BeginPlay() override;
    virtual void OnDeath() override;
	virtual void OnHealthChanged(float Health, float HealthData) override;

private:
	void UpdateHealthWidgetVisibility();
};
