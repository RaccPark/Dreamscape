// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSTutorialWidget.generated.h"

/**
 * 튜토리얼 UI 위젯 C++ 베이스 클래스.
 * Blueprint(WBP_Tutorial)에서 상속하여 비주얼 구현.
 */
UCLASS()
class DREAMSCAPE_API UDSTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Blueprint에서 구현 - 현재 튜토리얼 단계 텍스트 업데이트
	UFUNCTION()
	void SetTutorialContent(const FText& StepTitle, const FText& StepDescription);

	// Blueprint에서 구현 - 단계 완료 애니메이션 재생
	UFUNCTION()
	void PlayStepCompleteAnimation();

	// Blueprint에서 구현 - 전체 튜토리얼 완료 시 호출
	UFUNCTION()
	void PlayTutorialCompleteAnimation();
};
