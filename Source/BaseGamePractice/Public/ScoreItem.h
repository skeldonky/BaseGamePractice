// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ScoreItem.generated.h"

/**
 * 
 */
UCLASS()
class BASEGAMEPRACTICE_API AScoreItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AScoreItem();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
	
	virtual void ActivateItem(AActor* Activator) override;
};
