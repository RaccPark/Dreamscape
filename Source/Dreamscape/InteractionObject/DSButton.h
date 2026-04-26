// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DSInteractableInterface.h"
#include "DSButton.generated.h"

UCLASS()
class DREAMSCAPE_API ADSButton : public AActor, public IDSInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> ButtonMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<class UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TSubclassOf<class UDSInteractoinWidget> InteractionWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<class UDSInteractoinWidget> InteractionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TArray<AActor*> LinkedActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	uint8 bIsActiveOnlyOnce : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FString ButtonTip;

	bool bWasInteracted;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(ADSCharacterPlayer* Player) override;

	virtual void OnFocus() override;
	virtual void OnFocusLost() override;
};
