// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystem/DSTutorialSubsystem.h"
#include "Character/DSCharacterPlayer.h"
#include "Components/DSPlayerFSMComponent.h"
#include "Components/DSInteractionComponent.h"
#include "GameMode/DSGameState.h"
#include "UI/DSTutorialWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UDSTutorialSubsystem::Deinitialize()
{
	if (TutorialWidget && TutorialWidget->IsInViewport())
	{
		TutorialWidget->RemoveFromParent();
	}

	Super::Deinitialize();
}

void UDSTutorialSubsystem::BindAndStart(ADSCharacterPlayer* Player)
{
	if (!Player)
	{
		return;
	}

	CachedPlayer = Player;

	// --- 위젯 클래스 로드 (BP에서 WBP_Tutorial 생성 후 경로 일치 필요) ---
	TutorialWidgetClass = LoadClass<UDSTutorialWidget>(
		nullptr,
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Tutorial.WBP_Tutorial_C'"));

	// --- 이벤트 큐 초기화 ---
	StepQueue = {
		EDSTutorialStep::Movement,
		EDSTutorialStep::CameraPeek,
		EDSTutorialStep::Attack,
		EDSTutorialStep::Interact,
		EDSTutorialStep::DestroyCrystal,
	};
	StepIndex = 0;
	bTutorialActive = true;
	bCurrentStepCompleted = false;
	CurrentStep = StepQueue[0];

	// --- 플레이어 FSM 바인딩 (Movement, Attack 감지) ---
	if (UDSPlayerFSMComponent* FSM = Player->GetPlayerFSMComponent())
	{
		FSM->OnStateChangedDelegate.AddDynamic(this, &UDSTutorialSubsystem::OnPlayerStateChanged);
	}

	// --- 카메라 피킹 바인딩 ---
	Player->OnCameraPeekStartedDelegate.AddUObject(this, &UDSTutorialSubsystem::OnCameraPeekUsed);

	// --- 상호작용 바인딩 ---
	if (UDSInteractionComponent* InteractionComp = Player->GetInteractionComponent())
	{
		InteractionComp->OnInteractedDelegate.AddUObject(this, &UDSTutorialSubsystem::OnInteracted);
	}

	// --- 크리스탈 파괴 바인딩 ---
	if (ADSGameState* GS = Cast<ADSGameState>(UGameplayStatics::GetGameState(Player)))
	{
		GS->OnCrystalDestroyedDelegate.AddDynamic(this, &UDSTutorialSubsystem::OnCrystalDestroyed);
	}

	ShowCurrentStep();
}

void UDSTutorialSubsystem::ShowCurrentStep()
{
	APlayerController* PC = GetLocalPlayer()->GetPlayerController(GetWorld());
	if (!PC)
	{
		return;
	}

	// 디버그 텍스트 출력 (키 1 = 제목, 키 2 = 설명)
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 999.f, FColor::Yellow,
			FString::Printf(TEXT("[튜토리얼] %s"), *GetStepTitle(CurrentStep).ToString()));
		GEngine->AddOnScreenDebugMessage(2, 999.f, FColor::White,
			FString::Printf(TEXT("%s"), *GetStepDescription(CurrentStep).ToString()));
	}

	// 위젯 최초 생성
	if (!TutorialWidget && TutorialWidgetClass)
	{
		TutorialWidget = CreateWidget<UDSTutorialWidget>(PC, TutorialWidgetClass);
		if (TutorialWidget)
		{
			TutorialWidget->AddToViewport(10);
		}
	}

	if (TutorialWidget)
	{
		TutorialWidget->SetTutorialContent(GetStepTitle(CurrentStep), GetStepDescription(CurrentStep));
	}

	OnTutorialStepChangedDelegate.Broadcast(CurrentStep, GetStepDescription(CurrentStep));
}

void UDSTutorialSubsystem::OnStepCompleted()
{
	if (!bTutorialActive || bCurrentStepCompleted)
	{
		return;
	}
	bCurrentStepCompleted = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Green, TEXT("단계 완료!"));
	}

	if (TutorialWidget)
	{
		TutorialWidget->PlayStepCompleteAnimation();
	}

	// 완료 애니메이션 후 다음 단계로 넘어가도록 딜레이
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDSTutorialSubsystem::AdvanceToNextStep, 1.5f, false);
}

void UDSTutorialSubsystem::AdvanceToNextStep()
{
	StepIndex++;

	if (StepIndex >= StepQueue.Num())
	{
		// 모든 튜토리얼 완료
		bTutorialActive = false;
		CurrentStep = EDSTutorialStep::Completed;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Cyan, TEXT("[튜토리얼 완료]"));
			GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Cyan, TEXT("모든 튜토리얼을 완료했습니다!"));
		}

		if (TutorialWidget)
		{
			TutorialWidget->PlayTutorialCompleteAnimation();
		}

		OnTutorialAllCompletedDelegate.Broadcast();
		return;
	}

	CurrentStep = StepQueue[StepIndex];
	bCurrentStepCompleted = false;
	ShowCurrentStep();
}

// 완료 조건 감지 콜백
void UDSTutorialSubsystem::OnPlayerStateChanged(EPlayerStateType NewState)
{
	if (!bTutorialActive || bCurrentStepCompleted)
	{
		return;
	}

	if (CurrentStep == EDSTutorialStep::Movement && NewState == EPlayerStateType::EPS_Walk)
	{
		OnStepCompleted();
	}
	else if (CurrentStep == EDSTutorialStep::Attack && NewState == EPlayerStateType::EPS_SwordAttack)
	{
		OnStepCompleted();
	}
}

void UDSTutorialSubsystem::OnCameraPeekUsed()
{
	if (!bTutorialActive || bCurrentStepCompleted)
	{
		return;
	}

	if (CurrentStep == EDSTutorialStep::CameraPeek)
	{
		OnStepCompleted();
	}
}

void UDSTutorialSubsystem::OnInteracted()
{
	if (!bTutorialActive || bCurrentStepCompleted)
	{
		return;
	}

	if (CurrentStep == EDSTutorialStep::Interact)
	{
		OnStepCompleted();
	}
}

void UDSTutorialSubsystem::OnCrystalDestroyed(int32 CurrentCount)
{
	if (!bTutorialActive || bCurrentStepCompleted)
	{
		return;
	}

	if (CurrentStep == EDSTutorialStep::DestroyCrystal)
	{
		OnStepCompleted();
	}
}

// 텍스트 데이터
FText UDSTutorialSubsystem::GetStepTitle(EDSTutorialStep Step) const
{
	switch (Step)
	{
	case EDSTutorialStep::Movement:			return FText::FromString(TEXT("이동"));
	case EDSTutorialStep::CameraPeek:		return FText::FromString(TEXT("카메라 피킹"));
	case EDSTutorialStep::Attack:			return FText::FromString(TEXT("공격"));
	case EDSTutorialStep::Interact:			return FText::FromString(TEXT("상호작용"));
	case EDSTutorialStep::DestroyCrystal:	return FText::FromString(TEXT("크리스탈 파괴"));
	default:								return FText::GetEmpty();
	}
}

FText UDSTutorialSubsystem::GetStepDescription(EDSTutorialStep Step) const
{
	switch (Step)
	{
	case EDSTutorialStep::Movement:
		return FText::FromString(TEXT("WASD 키로 이동하세요."));
	case EDSTutorialStep::CameraPeek:
		return FText::FromString(TEXT("Shift 키를 눌러 카메라를 피킹하세요."));
	case EDSTutorialStep::Attack:
		return FText::FromString(TEXT("마우스 좌클릭으로 공격하세요."));
	case EDSTutorialStep::Interact:
		return FText::FromString(TEXT("E 키로 오브젝트와 상호작용하세요."));
	case EDSTutorialStep::DestroyCrystal:
		return FText::FromString(TEXT("드림 크리스탈을 공격하여 파괴하세요."));
	default:
		return FText::GetEmpty();
	}
}
