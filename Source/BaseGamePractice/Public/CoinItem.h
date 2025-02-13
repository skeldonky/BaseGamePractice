// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScoreItem.h"
#include "CoinItem.generated.h"

/**
 * 
 */
UCLASS()
class BASEGAMEPRACTICE_API ACoinItem : public AScoreItem
{
	GENERATED_BODY()
	
public:
	ACoinItem();

	virtual void ActivateItem(AActor* Activator) override;
};
