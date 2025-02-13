// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ItemSpawner.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class BASEGAMEPRACTICE_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMyGameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWaves; // 최대 웨이브 수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveDuration;	//웨이브 시간

	int32 Score;
	int32 SpawnedCoinCount;
	int32 CollectedCoinCount;
	int32 CurrentWaveIndex;
	
	FTimerHandle WaveTimerHandle;			//웨이브 타이머 핸들
	FTimerHandle HUDUpdateTimerHandle;		// UI

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	AItemSpawner* ItemSpawner;

	void StartNextWave();               // 다음 웨이브 시작
	void StartPreparationPhase(); // 준비 단계 시작
	void EndPreparationPhase();   // 준비 단계 종료 및 웨이브 시작

	UFUNCTION(BlueprintCallable, Category = "Wave")
	int32 GetMaxWaves() const;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void OnCoinCollected();
	void EndWave();
	void UpdateHUD();
};
