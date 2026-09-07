// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Components/DSPlayerFSMComponent.h"
#include "DSTutorialSubsystem.generated.h"

UENUM(BlueprintType)
enum class EDSTutorialStep : uint8
{
	Movement		UMETA(DisplayName = "기본 움직임"),
	CameraPeek		UMETA(DisplayName = "카메라 피킹"),
	Attack			UMETA(DisplayName = "공격"),
	Interact		UMETA(DisplayName = "상호작용"),
	DestroyCrystal	UMETA(DisplayName = "크리스탈 파괴"),
	Completed		UMETA(DisplayName = "완료"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTutorialStepChanged, EDSTutorialStep, NewStep, FText, StepDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTutorialAllCompleted);

/**
 * 튜토리얼 이벤트 큐를 관리하는 LocalPlayer 서브시스템.
 * ADSCharacterPlayer::BeginPlay에서 BindAndStart()를 호출하여 시작.
 *
 * 완료 조건:
 *   Movement       - 플레이어가 Walk 상태 진입
 *   CameraPeek     - Shift 키(카메라 피킹) 사용
 *   Attack         - SwordAttack 상태 진입
 *   Interact       - E 키 상호작용 실행
 *   DestroyCrystal - DreamCrystal 1개 이상 파괴
 */
UCLASS()
class DREAMSCAPE_API UDSTutorialSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	// 플레이어 BeginPlay에서 호출 - 델리게이트 바인딩 후 튜토리얼 시작
	void BindAndStart(class ADSCharacterPlayer* Player);

	UPROPERTY(BlueprintAssignable)
	FOnTutorialStepChanged OnTutorialStepChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnTutorialAllCompleted OnTutorialAllCompletedDelegate;

	UFUNCTION(BlueprintCallable)
	EDSTutorialStep GetCurrentStep() const { return CurrentStep; }

	UFUNCTION(BlueprintCallable)
	bool IsTutorialActive() const { return bTutorialActive; }

private:
	void ShowCurrentStep();
	void OnStepCompleted();
	void AdvanceToNextStep();

	// FSM 상태 변경 감지 (Movement, Attack 조건)
	UFUNCTION()
	void OnPlayerStateChanged(EPlayerStateType NewState);

	// DreamCrystal 파괴 감지
	UFUNCTION()
	void OnCrystalDestroyed(int32 CurrentCount);

	// 카메라 피킹 감지 (FSimpleMulticastDelegate 바인딩)
	void OnCameraPeekUsed();

	// 상호작용 실행 감지 (FSimpleMulticastDelegate 바인딩)
	void OnInteracted();

	FText GetStepTitle(EDSTutorialStep Step) const;
	FText GetStepDescription(EDSTutorialStep Step) const;

	// 큐 전체를 조회할 수 있게 하여 디버깅과 UI 표시에 용이하도록
	TArray<EDSTutorialStep> StepQueue;	// 이벤트 큐
	int32 StepIndex;					// 현재 큐 인덱스
	EDSTutorialStep CurrentStep;		// 현재 단계
	bool bTutorialActive;
	bool bCurrentStepCompleted;			// 중복 완료 방지

	UPROPERTY()
	TObjectPtr<class ADSCharacterPlayer> CachedPlayer;

	UPROPERTY()
	TSubclassOf<class UDSTutorialWidget> TutorialWidgetClass;

	UPROPERTY()
	TObjectPtr<class UDSTutorialWidget> TutorialWidget;
};
