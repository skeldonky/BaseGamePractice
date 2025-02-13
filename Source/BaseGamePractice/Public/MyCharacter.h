// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;	// 스프링 암
class UCameraComponent;		// 카메라

// Enhanced Input 구조체
struct FInputActionValue;

UCLASS()
class BASEGAMEPRACTICE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// 스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

	// 버프 함수
	UFUNCTION()
	void ApplyBuff(FName BuffType, float Duration);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//IA_Move와 IA_Jump 등을 처리할 함수 원형
	//Enhanced Input 에서 액션 값은 FInputActionValue로 전달됩니다.
	UFUNCTION()
	void Move(const FInputActionValue& value);

	UFUNCTION()
	void StartJump(const FInputActionValue& value);

	UFUNCTION()
	void StopJump(const FInputActionValue& value);

	UFUNCTION()
	void Look(const FInputActionValue& value);

	UFUNCTION()
	void StartSprint(const FInputActionValue& value);

	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	FTimerHandle BuffTimerHandle;

	void ResetSpeed();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	float NormalSpeed; // 기본 걷기 속도
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	float SprintSpeed; 	// 실제 스프린트 속도
};
