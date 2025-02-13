// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawner.h"
#include "MyGameInstance.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);
}

void AItemSpawner::SpawnRandomItem()
{
    if (FItemSpawnRow* SelectedRow = GetRandomItem())
    {
        if (UClass* ActualClass = SelectedRow->ItemClass.Get())
        {
            SpawnItem(ActualClass);
        }
    }
}

FVector AItemSpawner::GetRandomPointInVolume() const
{
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

FItemSpawnRow* AItemSpawner::GetRandomItem() const
{
    int32 WaveIndex = 0;
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            WaveIndex = MyGameInstance->CurrentWave;
        }
    }

    if (!ItemDataTable.IsValidIndex(WaveIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid WaveIndex: %d"), WaveIndex);
        return nullptr;
    }

    UDataTable* SelectedTable = ItemDataTable[WaveIndex];
    if (!SelectedTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("No DataTable assigned for WaveIndex: %d"), WaveIndex);
        return nullptr;
    }

    TArray<FItemSpawnRow*> AllRows;
    static const FString ContextString(TEXT("ItemSpawnContext"));
    SelectedTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No rows found in DataTable for WaveIndex: %d"), WaveIndex);
        return nullptr;
    }

    float TotalChance = 0.0f;
    for (const FItemSpawnRow* Row : AllRows)
    {
        if (Row)
        {
            TotalChance += Row->SpawnChance;
        }
    }

    const float RandValue = FMath::FRandRange(0.0f, TotalChance);
    float AccumulateChance = 0.0f;

    for (FItemSpawnRow* Row : AllRows)
    {
        if (Row)
        {
            AccumulateChance += Row->SpawnChance;
            if (RandValue <= AccumulateChance)
            {
                return Row;
            }
        }
    }

    return nullptr;
}

void AItemSpawner::SpawnItem(TSubclassOf<AActor> ItemClass)
{
    if (!ItemClass) return;

    GetWorld()->SpawnActor<AActor>(
        ItemClass,
        GetRandomPointInVolume(),
        FRotator::ZeroRotator
    );
}


