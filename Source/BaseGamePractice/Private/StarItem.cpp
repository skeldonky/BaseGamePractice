// Fill out your copyright notice in the Description page of Project Settings.


#include "StarItem.h"

AStarItem::AStarItem()
{
	PointValue = 200;
	ItemType = "Star";
}

void AStarItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
