// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const {
    //наследуем из базового класса обычную скорость
    const float MaxSpeed = Super::GetMaxSpeed();
    //переменная player 
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
    //если переменная валидная и игрок бежит
    return Player && Player->IsRunnig() ? MaxSpeed * RunModifier : MaxSpeed;
 }