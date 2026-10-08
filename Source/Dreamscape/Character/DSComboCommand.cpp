#include "Character/DSComboCommand.h"
#include "Components/DSPlayerCombatComponent.h"

void DSSwordAttackCommand::Execute(UDSPlayerCombatComponent* Combat)
{
	if (Combat)
	{
		UE_LOG(LogTemp, Log, TEXT("[Command] DSSwordAttackCommand::Execute -> ComboActionBegin() 호출"));
		Combat->ComboActionBegin();
	}
}
