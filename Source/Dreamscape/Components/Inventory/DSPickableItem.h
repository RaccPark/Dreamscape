// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DSInteractableInterface.h"
#include "DSPickableItem.generated.h"

UCLASS()
class DREAMSCAPE_API ADSPickableItem : public AActor, public IDSInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSPickableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UStaticMeshComponent> ItemMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<class UDSItemData> ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<class UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TSubclassOf<class UDSInteractoinWidget> InteractionWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<class UDSInteractoinWidget> InteractionWidget;


		
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class ADSCharacterPlayer* Player) override;

	virtual void OnFocus() override;
	virtual void OnFocusLost() override;
};
