// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScoreItem.h"
#include "StarItem.generated.h"

/**
 * 
 */
UCLASS()
class BASEGAMEPRACTICE_API AStarItem : public AScoreItem
{
	GENERATED_BODY()
	
public:
	AStarItem();

	virtual void ActivateItem(AActor* Activator) override;
};
