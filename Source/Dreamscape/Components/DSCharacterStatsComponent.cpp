// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSCharacterStatsComponent.h"

UDSCharacterStatsComponent::UDSCharacterStatsComponent()
{
	// 스탯은 이벤트로만 바뀐다. 매 프레임 할 일이 없다.
	PrimaryComponentTick.bCanEverTick = false;

	// 기존 ADSEnemyCharacterBase의 기본 체력과 같은 값
	BaseMaxHealth = 3;
}

void UDSCharacterStatsComponent::InitializeStats()
{
	// 베이스는 에디터에서 입력된 BaseMaxHealth를 그대로 쓴다.
	OnStatsChanged.Broadcast();
}

int32 UDSCharacterStatsComponent::GetMaxHealth() const
{
	return BaseMaxHealth;
}
