// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnLevel.h"

#include "BaseLevel.h"
#include "Engine.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnLevel::ASpawnLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnLevel::BeginPlay()
{
	Super::BeginPlay();

	SpawnLevel(true);
	SpawnLevel(false);
	SpawnLevel(false);
	SpawnLevel(false);

}

// Called every frame
void ASpawnLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnLevel::SpawnLevel(bool isFirst)
{
	UE_LOG(LogTemp, Error, TEXT("Create Level"));

	spawnLocation = FVector(0.0f, 1000.0f, 0.0f);
	spawnRotation = FRotator(0, 90, 0);
	

	if (!isFirst)
	{
		ABaseLevel* LastLevel = LevelList.Last(); 
		spawnLocation = LastLevel->GetSpawnLocation()->GetComponentTransform().GetTranslation();
	}

	RandomLevel = FMath::RandRange(1, 10); 
	ABaseLevel* newLevel = nullptr; 
	UE_LOG(LogTemp, Error, TEXT("Level Num : %d"),RandomLevel);

	if (RandomLevel == 1)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level1,
			spawnLocation, spawnRotation, SpawnInfo);
	}
	else if (RandomLevel == 2)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level2,
			spawnLocation, spawnRotation, SpawnInfo);
	}
	else if (RandomLevel == 3)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level3,
			spawnLocation, spawnRotation, SpawnInfo);
	}
	else if (RandomLevel == 4)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level4,
			spawnLocation, spawnRotation, SpawnInfo);
	}
	else if (RandomLevel == 5)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level5,
			spawnLocation, spawnRotation, SpawnInfo);
	}
	else if (RandomLevel == 6)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level6,
			spawnLocation, spawnRotation, SpawnInfo);
	}
	else if (RandomLevel == 7)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level7,
			spawnLocation, spawnRotation, SpawnInfo);
	}
	else if (RandomLevel == 8)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level8,
			spawnLocation, spawnRotation, SpawnInfo);
	}
	else if (RandomLevel == 9)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level9,
			spawnLocation, spawnRotation, SpawnInfo);
	}
	else if (RandomLevel == 10)
	{
		newLevel = GetWorld()->SpawnActor<ABaseLevel>(Level10,
			spawnLocation, spawnRotation, SpawnInfo);
	}

	if (newLevel != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Level Not Null"));
		if (newLevel->GetTrigger() != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Trigger Setting"));

			newLevel->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &ASpawnLevel::OnOverlapBegin); 
		 }
	}

	LevelList.Add(newLevel); 
	if (LevelList.Num() > 5)
	{
		LevelList.RemoveAt(0); 
	}

}	

void ASpawnLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SpawnLevel(false);
}

