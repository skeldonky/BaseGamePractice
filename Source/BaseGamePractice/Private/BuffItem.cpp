// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffItem.h"
#include "MyCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABuffItem::ABuffItem()
{
	BuffName = "DefaultBuff";
	ItemType = "BuffItem";
	BuffDuration = 0.f;

	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ABuffItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	ActiveBuff();
	DestroyItem();
}

void ABuffItem::ActiveBuff()
{
	TArray<AActor*> OverlappingActors;
	Collision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			// ĳ���Ͱ� AMyCharacter Ÿ������ Ȯ��
			if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(Actor))
			{
				// MyCharacter���� ������ ����
				MyCharacter->ApplyBuff(BuffName, BuffDuration);

				GEngine->AddOnScreenDebugMessage(
					-1,
					2.0f,
					FColor::Green,
					FString::Printf(TEXT("Player received buff: %s for %f seconds"), *BuffName.ToString(), BuffDuration)
				);
			}
		}
	}
}
