// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100.0f;
	Health = MaxHealth;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Enhanced inputComponent로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//IA 를 가져오기 위해 현재 소유중인 Controller를 ASpartaPlayerController로 캐스팅
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				//IA_Move 액션 키를 누르고 있는 동안 Move() 호출
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
			}

			if (PlayerController->JumpAction)
			{
				//IA_Jump 액션 키를 누르고있는동안 StartJump() 호출
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartJump);

				//IA_Jump 액션 키를 때면은 StopJump() 호출
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::StopJump);
			}

			if (PlayerController->LookAction)
			{
				//IA_Look 액션 마우스가 움직일때 Look() 호출
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
			}

			if (PlayerController->SprintAction)
			{
				//IA_Sprint 액션 키를 누르고 있는 동안 StartSprint() 호출
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartSprint);

				//IA_Sprint 액션 키를 때면은 StopSprint() 호출
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::StopSprint);
			}
		}
	}
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller)
	{
		return;
	}

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void AMyCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void AMyCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	//좌우 회전
	AddControllerYawInput(LookInput.X);
	//상하 회전
	AddControllerPitchInput(LookInput.Y);
}

void AMyCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMyCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void AMyCharacter::ResetSpeed()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void AMyCharacter::OnDeath()
{
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	// 실제 적용된 데미지를 반환
	return ActualDamage;
}

int32 AMyCharacter::GetHealth() const
{
	return Health;
}

void AMyCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
}

void AMyCharacter::ApplyBuff(FName BuffType, float Duration)
{
	// BuffType에 따라 다른 효과를 처리
	if (BuffType == "SpeedBoost")
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
			GetWorld()->GetTimerManager().SetTimer(BuffTimerHandle, this, &AMyCharacter::ResetSpeed, Duration, false);
		}
	}
}

