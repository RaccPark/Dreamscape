#pragma once

class ADSCharacterPlayer;

// Command 인터페이스
class IDSComboCommand
{
public:
	virtual ~IDSComboCommand() = default;
	virtual void Execute(ADSCharacterPlayer* Player) = 0;
};

// ConcreteCommand: 검 공격 콤보 시작
class DSSwordAttackCommand : public IDSComboCommand
{
public:
	virtual void Execute(ADSCharacterPlayer* Player) override;
};
