// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAMSCAPE_API UDSInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDSInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* CurrentActor;

	TScriptInterface<class IDSInteractableInterface> CurrentInteractable;

	UPROPERTY(EditAnywhere)
	float InteractionRange;
	
	UPROPERTY(EditAnywhere)
	float DetectionRadius;

	void MoveToTarget();
	void ExecuteInteraction();

private:
	void UpdateInteractable();
	void SetCurrentActor(AActor* NewActor);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TryInteract();

	// 상호작용 실행 시 브로드캐스트 (튜토리얼 감지용)
	FSimpleMulticastDelegate OnInteractedDelegate;
};
