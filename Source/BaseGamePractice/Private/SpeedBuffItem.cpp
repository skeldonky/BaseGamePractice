// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBuffItem.h"

ASpeedBuffItem::ASpeedBuffItem()
{
	BuffName = "SpeedBoost";
	BuffDuration = 20.0f;
}

void ASpeedBuffItem::ActivateItem(AActor* Activator)
{
	DestroyItem();
}
