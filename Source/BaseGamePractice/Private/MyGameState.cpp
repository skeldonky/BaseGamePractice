// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AMyGameState::AMyGameState()
{
    MaxWaves = 3;
    CurrentWaveIndex = 0;
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
    WaveDuration = 0.0f;
}

void AMyGameState::BeginPlay()
{
    Super::BeginPlay();

    // 첫 웨이브 준비 단계 시작
    StartPreparationPhase();

    // UI 0.1초마다 갱신
    GetWorldTimerManager().SetTimer(
        HUDUpdateTimerHandle,
        this,
        &AMyGameState::UpdateHUD,
        0.1f,
        true
    );
}

void AMyGameState::StartNextWave()
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            CurrentWaveIndex = MyGameInstance->CurrentWave;
        }
    }

    if (CurrentWaveIndex >= MaxWaves)
    {
        if (GEngine)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("모든 웨이브 완료!")));
        }
        return; // 모든 웨이브 완료
    }

    SpawnedCoinCount = 0;
    CollectedCoinCount = 0;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Wave %d 시작!"), CurrentWaveIndex));
    }

    // 웨이브 시간
    WaveDuration = 5.0f;

    // 다음 웨이브 준비 단계를 위한 타이머 설정
    if (CurrentWaveIndex < MaxWaves)
    {
        GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AMyGameState::EndWave, WaveDuration, false);
    }

    UpdateHUD();
}

void AMyGameState::StartPreparationPhase()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Wave %d 준비 중... %d초 후 시작!"), CurrentWaveIndex + 1, FMath::RoundToInt(WaveDuration)));
    }

    // 대기시간으로 변경
    WaveDuration = 10.0f;

    for (int32 i = 0; i < 10; ++i)
    {
        if (ItemSpawner)
        {
            ItemSpawner->SpawnRandomItem();
        }
    }

    // 준비 시간이 끝나면 웨이브 시작
    GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AMyGameState::EndPreparationPhase, WaveDuration, false);
    UpdateHUD();
}

void AMyGameState::EndPreparationPhase()
{
    // 준비 단계가 끝나면 웨이브 시작
    StartNextWave();
}

int32 AMyGameState::GetMaxWaves() const
{
    return MaxWaves;
}

int32 AMyGameState::GetScore() const
{
	return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            MyGameInstance->AddToScore(Amount);
        }
    }
}

void AMyGameState::OnGameOver()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
        {
            MyPlayerController->SetPause(true);
            //MyPlayerController->ShowMainMenu(true);
        }
    }
}

void AMyGameState::OnCoinCollected()
{
    CollectedCoinCount++;

    if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
    {
        EndWave();
    }
}

void AMyGameState::EndWave()
{
    GetWorldTimerManager().ClearTimer(WaveTimerHandle);

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            AddScore(Score);
            CurrentWaveIndex++;
            MyGameInstance->CurrentWave = CurrentWaveIndex;

            ItemSpawner->DestroyAllSpawnedItems();

            if (CurrentWaveIndex >= MaxWaves)
            {
                OnGameOver();
                return;
            }
            else
            {
                StartPreparationPhase();
            }
        }
    }
}

void AMyGameState::UpdateHUD()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        AMyPlayerController* MyController = Cast<AMyPlayerController>(PlayerController);
        {
            if (UUserWidget* HUDWidget = MyController->GetHUDWidget())
            {
                if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText"))))
                {
                    float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
                    TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time : %.1f"), RemainingTime)));
                }

                if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText"))))
                {
                    if (UGameInstance* GameInstance = GetGameInstance())
                    {
                        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
                        if (MyGameInstance)
                        {
                            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), MyGameInstance->TotalScore)));
                        }
                    }
                }

                if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("WaveText"))))
                {
                    WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave : %d"), CurrentWaveIndex + 1)));
                }

                if (UTextBlock* HPText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HPText"))))
                {
                    // AMyCharacter를 직접 가져옴
                    if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(PlayerController->GetCharacter()))
                    {
                        int32 Health = MyCharacter->GetHealth(); // 캐릭터의 현재 체력
                        HPText->SetText(FText::FromString(FString::Printf(TEXT("Health : %d / 100"), Health)));
                    }
                }
            }
        }
    }
}


