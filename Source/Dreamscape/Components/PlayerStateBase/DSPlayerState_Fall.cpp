// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_Fall.h"

void UDSPlayerState_Fall::Enter()
{
}

void UDSPlayerState_Fall::Update(float DeltaTime)
{
}

void UDSPlayerState_Fall::Exit()
{
}

void UDSPlayerState_Fall::OnSwordAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("낙하 중 공격!"));
}

void UDSPlayerState_Fall::OnDeath()
{
}
