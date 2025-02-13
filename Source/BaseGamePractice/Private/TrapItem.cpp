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

	// 5초 후 폭발 실행
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
			// 데미지를 발생시켜 Actor->TakeDamage()가 실행되도록 함
			UGameplayStatics::ApplyDamage(
				Actor,                      // 데미지를 받을 액터
				ExplosionDamage,            // 데미지 양
				nullptr,                    // 데미지를 유발한 주체 (지뢰를 설치한 캐릭터가 없으므로 nullptr)
				this,                       // 데미지를 유발한 오브젝트(지뢰)
				UDamageType::StaticClass()  // 기본 데미지 유형
			);

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player damaged %d by TrapItem"), ExplosionDamage));
		}
	}

	// 지뢰 제거
	DestroyItem();
}
