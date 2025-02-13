// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreItem.h"
#include "MyGameState.h"

AScoreItem::AScoreItem()
{
	PointValue = 0;
	ItemType = "DefaultScore";
}

void AScoreItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	UE_LOG(LogTemp, Log, TEXT("ActiveS!!!"));
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AMyGameState* GameState = World->GetGameState<AMyGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d points!"), PointValue));

		DestroyItem();
	}
}
