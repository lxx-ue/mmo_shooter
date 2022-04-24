// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/STUAnimNotify.h"


void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp); //вызываем делегат
    Super::Notify(MeshComp, Animation); //наследуем базовую логику
}