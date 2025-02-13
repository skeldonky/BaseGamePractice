// Fill out your copyright notice in the Description page of Project Settings.


#include "BombItem.h"

ABombItem::ABombItem()
{
	ExplosionDelay = 0.1f;
	ExplosionDamage = 30.0f;
	ItemType = "Bomb";
}

void ABombItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
