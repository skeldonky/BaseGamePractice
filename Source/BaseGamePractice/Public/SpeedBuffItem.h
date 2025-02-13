// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffItem.h"
#include "SpeedBuffItem.generated.h"

/**
 * 
 */
UCLASS()
class BASEGAMEPRACTICE_API ASpeedBuffItem : public ABuffItem
{
	GENERATED_BODY()
	
public:
	ASpeedBuffItem();

	virtual void ActivateItem(AActor* Activator) override;
};
