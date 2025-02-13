// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BuffItem.generated.h"

/**
 * 
 */
UCLASS()
class BASEGAMEPRACTICE_API ABuffItem : public ABaseItem
{
	GENERATED_BODY()

public:

	ABuffItem();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName BuffName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float BuffDuration;

	virtual void ActivateItem(AActor* Activator) override;

	void ActiveBuff();
};
