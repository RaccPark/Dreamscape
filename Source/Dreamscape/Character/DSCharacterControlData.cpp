// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DSCharacterControlData.h"

UDSCharacterControlData::UDSCharacterControlData()
{
	TargetArmLength = 400.f;
	FOV = 8.0f;		// 아이소메트릭 탑뷰: 거리는 멀되 FOV는 4~10
	MaxWalkSpeed = 225.0f;
}
