// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpikes.h"

AWallSpikes::AWallSpikes()
{
	PrimaryActorTick.bCanEverTick = true; 
}

void AWallSpikes::BeginPlay()
{
	Super::BeginPlay(); 

	this->GetRootComponent()->ComponentVelocity = FVector(0, 25, 0); 

}

void AWallSpikes::Tick(float deltaTime)
{
	Super::Tick(deltaTime); 
	SetActorLocation(GetActorLocation() + FVector(0, 350 * deltaTime, 0), true); 
}
