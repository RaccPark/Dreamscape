// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSCameraPeekComponent.generated.h"

// Camera Peek Component
// 
// Shift 키 입력 시 카메라를 마우스 방향으로 이동시켜 주변을 살펴볼 수 있는 컴포넌트
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAMSCAPE_API UDSCameraPeekComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDSCameraPeekComponent();

	void SetPeekActive(bool bActive);
	void AddMouseDelta(const FVector2D& Delta);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY()
	TObjectPtr<class UCameraComponent> CameraComponent;

	// Status
	bool bIsPeeking;
	FVector2D AccumulatedMouse;	// MouseXY는 델타 입력이므로, 방향 누적을 위해 누적 사용

	// Config

	// 최대 피크 거리
	UPROPERTY(EditAnywhere, Category = "Camera Peek")
	float MaxPeekDistance;	

	UPROPERTY(EditAnywhere, Category = "Camera Peek")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera Peek")
	float DeadZone;

	UPROPERTY(EditAnywhere, Category = "Camera Peek")
	float MaxInputRange;

	// Etc
	FVector CurrentOffset;

	void UpdatePeekOffset(float DeltaTime);
		
};
