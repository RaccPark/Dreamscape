#include "Character/DSComboCommand.h"
#include "Character/DSCharacterPlayer.h"

void DSSwordAttackCommand::Execute(ADSCharacterPlayer* Player)
{
	if (Player)
	{
		UE_LOG(LogTemp, Log, TEXT("[Command] DSSwordAttackCommand::Execute -> ComboActionBegin() 호출"));
		Player->ComboActionBegin();
	}
}
