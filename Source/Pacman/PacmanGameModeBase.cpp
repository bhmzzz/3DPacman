// Copyright Epic Games, Inc. All Rights Reserved.


#include "PacmanGameModeBase.h"
#include "Pacdot.h"
#include "PacmanEnemy.h"
#include "Engine/Public/EngineUtils.h"
#include "EnemyController.h"
#include "PacmanPlayer.h"


APacmanGameModeBase::APacmanGameModeBase() 
{
	CurrentState = EGameState::EMenu;
}

void APacmanGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//遍历场景中指定类的Actor
	for (TActorIterator<APacdot> PacdotItr(GetWorld()); PacdotItr; ++PacdotItr)
	{
		PacdotNum++;
	}

	for (TActorIterator<APacmanEnemy> EnemyItr(GetWorld()); EnemyItr; ++EnemyItr)
	{
		Enemies.Add(Cast<APacmanEnemy>(*EnemyItr));
	}
}

void APacmanGameModeBase::StartGame()
{
	if (CurrentState == EGameState::EMenu)
	{
		SetCurrentState(EGameState::EPlay);
		for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
		{
			Cast<AEnemyController>((*Iter)->GetController())->GotoNewDestination();
		}
	}
}

void APacmanGameModeBase::PauseGame()
{
	if (GetCurrentState() == EGameState::EPlay)
	{
		RecordEnemiesTimer();
		SetCurrentState(EGameState::EPause);
	}
	else if (GetCurrentState() == EGameState::EPause)
	{
		RestoreEnemiesTimer();
		SetCurrentState(EGameState::EPlay);
	}
}

void APacmanGameModeBase::RestartGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("RestartLevel"));
}

void APacmanGameModeBase::SetEnemiesVulnerable()
{
	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
	{
		(*Iter)->SetVulnerable();
	}
}

void APacmanGameModeBase::RecordEnemiesTimer()
{
	EnemiesTimerRemaining.Empty();//数组清空

	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
	{
		if ((*Iter)->bIsVulnerable == false && (*Iter)->bIsDead == false)
		{
			EnemiesTimerRemaining.Add(-1);
		}
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)
		{
			float temp = GetWorldTimerManager().GetTimerRemaining((*Iter)->VulnerableTimerHandle);
			EnemiesTimerRemaining.Add(temp);
		}
		if ((*Iter)->bIsDead == true)
		{
			float temp = GetWorldTimerManager().GetTimerRemaining((*Iter)->DeadTimerHandle);
			EnemiesTimerRemaining.Add(temp);
		}
	}
}

void APacmanGameModeBase::RestoreEnemiesTimer()
{
	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
	{
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)
		{
			(*Iter)->SetVulnerable(true, EnemiesTimerRemaining[Iter.GetIndex()]);
		}
		if ((*Iter)->bIsDead == true)
		{
			(*Iter)->SetDead(true, EnemiesTimerRemaining[Iter.GetIndex()]);
		}
	}
}

void APacmanGameModeBase::SetPacdotNum(int Value)
{
	PacdotNum = Value;
	if (PacdotNum == 0)
	{
		SetCurrentState(EGameState::EWin);
	}
}

void APacmanGameModeBase::SetCurrentState(EGameState Value)
{
	CurrentState = Value;
	switch (CurrentState)
	{
		case EGameState::EPlay:
			for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
			{
				(*Iter)->SetMovement(true);
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(true);
			break;
		case EGameState::EWin:
			for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
			{
				(*Iter)->Destroy();
			}
			break;
		case EGameState::EPause:
		case EGameState::EGameOver:
			for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
			{
				(*Iter)->SetMovement(false);
				GetWorldTimerManager().ClearAllTimersForObject(*Iter);
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(false);
			break;
		default:
			break;
	}
}



