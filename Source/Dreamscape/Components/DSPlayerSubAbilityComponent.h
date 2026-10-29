// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSPlayerSubAbilityComponent.generated.h"

// 보조 공격(충전 취소 포함)이 회복 시간까지 끝났을 때
// 캐릭터가 받아 FSM에 넘긴다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSubAbilityEnded);

/*
* 플레이어의 서브 공격을 담담하는 컴포넌트
* 서브 공격 전용 슬롯(0~3슬롯, 숫자키 1~4)에 각 능력을 하나씩 넣어 관리
* 충전 -> 발동(스테니마 소모) ->회복 순으로 진행
* 이때, 충전 상태에서 발동 취소 시 스테미나를 소모하지 않음
* 
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAMSCAPE_API UDSPlayerSubAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDSPlayerSubAbilityComponent();

	static const int32 SubAbilitySlotCount = 4;

protected:
	virtual void BeginPlay() override;

//	===
//	슬롯
//	===
public:
	// 슬롯 전환(0~3)
	// 충전 중인 서브 공격이 있으면 행동 취소 후 슬롯 전환
	// 회복 시간 중에도 슬롯 전환이 가능하다
	bool SelectSlot(int32 SlotIndex);

	const class UDSSubAbilityData* GetSelectedSubAbilityData() const;
	int32 GetSelectedSlotIndex() const;

//	===
//	충전, 발동
//	===
public:
	// 슬롯 데이터가 있는지, 충전/회복 중이 아닌지, 스테미나가 충분한지
	bool CanBeginCharge() const;

	void BeginCharge();
	// 충전 미완료시 취소, 완료시 발동
	void Release();
	// 충전 타이머 정리, 피격 시에도 호출
	void CancelCharge();

	bool IsCharging() const;
	bool IsChargingComplete() const;
	bool IsRecovering() const;

protected:
	void OnChargeComplete();

	// 서브 공격 종류별 발동 분기
	void ActivateSubAbility(const class UDSSubAbilityData* Data);
	// 서브 공격 종류
	void ActivateArrow(const class UDSSubAbilityData* Data);
	// void ActivateCone(const class UDSSubAbilityData* Data);
	// void ActivateExplosion(const class UDSSubAbilityData* Data);
	// void ActivateDash(const class UDSSubAbilityData* Data);

	void StartRecovery();
	void OnRecoveryComplete();

public:
	float GetChargeTime() const;
	float GetRecoveryTime() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "SubAbility")
	FOnSubAbilityEnded OnSubAbilityEnded;

//	===
//	상태
//	===
protected:
	// 숫자 키 1~4에 대응하는 슬롯 인덱스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubAbility")
	TArray<TObjectPtr<class UDSSubAbilityData>> SubAbilitySlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SubAbility")
	int32 SelectedSlotIndex;

	UPROPERTY()
	TObjectPtr<class ADSCharacterPlayer> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<class UDSPlayerStatsComponent> PlayerStatsComponent;

	UPROPERTY()
	TObjectPtr<class UDSPlayerVitalityComponent> PlayerVitalityComponent;

	FTimerHandle ChargeTimerHandle;
	FTimerHandle RecoveryTimerHandle;

	bool bIsCharging;
	bool bIsChargingComplete;
	bool bIsRecovering;	
};
