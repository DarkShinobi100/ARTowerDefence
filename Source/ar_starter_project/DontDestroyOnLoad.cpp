// Fill out your copyright notice in the Description page of Project Settings.


#include "DontDestroyOnLoad.h"

//This is a singular object and is used to store relevant data to be used in the player to unlock rewards
UDontDestroyOnLoad::UDontDestroyOnLoad()
{
	//set up default values
	m_MainGame = false;
	m_Loading = false;
	m_Scanning = false;
	m_LoadGoal = false;
	m_GamePlay = false;
	m_GameOver = false;
	m_GameOverButtons = false;
	m_SpawnEnemy = false;
}
//Game states
// Called to Set if the game is on the correct level
void UDontDestroyOnLoad::SetMainGame(bool NewValue)
{
	m_MainGame = NewValue;
}
// Called to Get if the game is on the correct level
bool UDontDestroyOnLoad::GetMainGame()
{
	return m_MainGame;
}

// Called to Set if the game is currently loading
void UDontDestroyOnLoad::SetLoading(bool NewValue)
{
	m_Loading = NewValue;
}

// Called to Get if the game is currently loading
bool UDontDestroyOnLoad::GetLoading()
{
	return m_Loading;
}

// Called to Set if the game is in the scanning mode for markers
void UDontDestroyOnLoad::SetScanning(bool NewValue)
{
	m_Scanning = NewValue;
}

// Called to Get if the game is in the scanning mode for markers
bool UDontDestroyOnLoad::GetScanning()
{
	return 	m_Scanning;
}

// Called to Set if the game is awaiting the player to set the goal
void UDontDestroyOnLoad::SetLoadGoal(bool NewValue)
{
	m_LoadGoal = NewValue;
}

// Called to Get if the game is awaiting the player to set the goal
bool UDontDestroyOnLoad::GetLoadGoal()
{
	return m_LoadGoal;
}

// Called to Set the goal location
void UDontDestroyOnLoad::SetGoalLocation(FVector location)
{
	m_GoalLocation = location;
}

// Called to Get the goal location
FVector UDontDestroyOnLoad::GetGoalLocation()
{
	return m_GoalLocation;
}

// Called to Set if the game is currently in the main gameplay state
void UDontDestroyOnLoad::SetGamePlay(bool NewValue)
{
	m_GamePlay = NewValue;
}

// Called to Get if the game is currently in the main gameplay state
bool UDontDestroyOnLoad::GetGamePlay()
{
	return 	m_GamePlay;
}

// Called to Set if the game is currently in the GameOver gameplay state
void UDontDestroyOnLoad::SetGameOver(bool NewValue)
{
	m_GameOver = NewValue;
}

// Called to Get if the game is currently in the GameOver gameplay state
bool UDontDestroyOnLoad::GetGameOver()
{
	return 	m_GameOver;
}

// Called to Set if the game is should display the GameOver menu buttons
void UDontDestroyOnLoad::SetGameOverButton(bool NewValue)
{
	m_GameOverButtons = NewValue;
}

// Called to Get if the game is should display the GameOver menu buttons
bool UDontDestroyOnLoad::GetGameOverButton()
{
	return m_GameOverButtons;
}

// Called to Set if the game should spawn more enemies
void UDontDestroyOnLoad::SetSpawnEnemy(bool NewValue)
{
	m_SpawnEnemy = NewValue;
}

// Called to Get if the game should spawn more enemies
bool UDontDestroyOnLoad::GetSpawnEnemy()
{
	return m_SpawnEnemy;
}

// Called to Set the enemies spawn location
void UDontDestroyOnLoad::SetSpawnLocation(FVector NewValue)
{
	m_SpawnLocation = NewValue;
}

// Called to Get the enemies spawn location
FVector UDontDestroyOnLoad::GetSpawnLocation()
{
	return m_SpawnLocation;
}

// Called to set Reset values
void UDontDestroyOnLoad::Reset()
{
	//set up default values
	m_MainGame = false;
	m_Loading = false;
	m_Scanning = false;
	m_LoadGoal = false;
	m_GamePlay = false;
	m_GameOver = false;
	m_GameOverButtons = false;
	m_SpawnEnemy = false;
}