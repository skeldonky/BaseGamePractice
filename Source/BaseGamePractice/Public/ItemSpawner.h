// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "ItemSpawner.generated.h"

class UBoxComponent;

UCLASS()
class BASEGAMEPRACTICE_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnRandomItem();

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<UDataTable*> ItemDataTable;

    FVector GetRandomPointInVolume() const;
    FItemSpawnRow* GetRandomItem() const;
    void SpawnItem(TSubclassOf<AActor> ItemClass);

};
