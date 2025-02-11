// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyGameState.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h" // 헤더 추가

AMyPlayerController::AMyPlayerController()
	:InputMappingContext(nullptr),
	 MoveAction(nullptr),
	 JumpAction(nullptr),
	 LookAction(nullptr),
	 SprintAction(nullptr),
	 HUDWidgetClass(nullptr),
	 HUDWidgetInstance(nullptr)
{
}

UUserWidget* AMyPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	// HUD 위젯 생성 및 표시
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}

	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateHUD();
	}
}
