// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"

ACoinItem::ACoinItem()
{
	PointValue = 50;
	ItemType = "Coin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
