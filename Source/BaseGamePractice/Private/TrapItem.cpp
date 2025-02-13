// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ATrapItem::ATrapItem()
{
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Trap";
	bHasExploded = false;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void ATrapItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (bHasExploded) return;

	// 5�� �� ���� ����
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &ATrapItem::Explode, ExplosionDelay, false);

	bHasExploded = true;
}

void ATrapItem::Explode()
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			// �������� �߻����� Actor->TakeDamage()�� ����ǵ��� ��
			UGameplayStatics::ApplyDamage(
				Actor,                      // �������� ���� ����
				ExplosionDamage,            // ������ ��
				nullptr,                    // �������� ������ ��ü (���ڸ� ��ġ�� ĳ���Ͱ� �����Ƿ� nullptr)
				this,                       // �������� ������ ������Ʈ(����)
				UDamageType::StaticClass()  // �⺻ ������ ����
			);

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player damaged %d by TrapItem"), ExplosionDamage));
		}
	}

	// ���� ����
	DestroyItem();
}
