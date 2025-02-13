// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapItem.h"
#include "BombItem.generated.h"

/**
 * 
 */
UCLASS()
class BASEGAMEPRACTICE_API ABombItem : public ATrapItem
{
	GENERATED_BODY()
	
public:

	ABombItem();

	virtual void ActivateItem(AActor* Activator) override;
};
