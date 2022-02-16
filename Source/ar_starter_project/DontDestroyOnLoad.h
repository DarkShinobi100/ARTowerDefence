// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DontDestroyOnLoad.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API UDontDestroyOnLoad : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// Sets default values for this objects's properties
	UDontDestroyOnLoad();

	// Called to Set if the game is on the correct level
	UFUNCTION(BlueprintCallable)
		void SetMainGame(bool NewValue);

	// Called to Get if the game is on the correct level
	UFUNCTION(BlueprintCallable)
		bool GetMainGame();

	// Called to Set if the game is currently loading
	UFUNCTION(BlueprintCallable)
		void SetLoading(bool NewValue);

	// Called to Get if the game is currently loading
	UFUNCTION(BlueprintCallable)
		bool GetLoading();

	// Called to Set if the game is in the scanning mode for markers
	UFUNCTION(BlueprintCallable)
		void SetScanning(bool NewValue);

	// Called to Get if the game is in the scanning mode for markers
	UFUNCTION(BlueprintCallable)
		bool GetScanning();

	// Called to Set if the game is awaiting the player to set the goal
	UFUNCTION(BlueprintCallable)
		void SetLoadGoal(bool NewValue);

	// Called to Get if the game is awaiting the player to set the goal
	UFUNCTION(BlueprintCallable)
		bool GetLoadGoal();

	// Called to Set the goal location
	UFUNCTION(BlueprintCallable)
		void SetGoalLocation(FVector location);

	// Called to Get the goal location
	UFUNCTION(BlueprintCallable)
		FVector GetGoalLocation();

	// Called to Set if the game is currently in the main gameplay state
	UFUNCTION(BlueprintCallable)
		void SetGamePlay(bool NewValue);

	// Called to Get if the game is currently in the main gameplay state
	UFUNCTION(BlueprintCallable)
		bool GetGamePlay();

	// Called to Set if the game is currently in the GameOver gameplay state
	UFUNCTION(BlueprintCallable)
		void SetGameOver(bool NewValue);

	// Called to Get if the game is currently in the GameOver gameplay state
	UFUNCTION(BlueprintCallable)
		bool GetGameOver();

	// Called to Set if the game is should display the GameOver menu buttons
	UFUNCTION(BlueprintCallable)
		void SetGameOverButton(bool NewValue);

	// Called to Get if the game is should display the GameOver menu buttons
	UFUNCTION(BlueprintCallable)
		bool GetGameOverButton();

	// Called to Set if the game should spawn more enemies
	UFUNCTION(BlueprintCallable)
		void SetSpawnEnemy(bool NewValue);

	// Called to Get if the game should spawn more enemies
	UFUNCTION(BlueprintCallable)
		bool GetSpawnEnemy();

	// Called to Set the enemies spawn location
	UFUNCTION(BlueprintCallable)
		void SetSpawnLocation(FVector NewValue);

	// Called to Get the enemies spawn location
	UFUNCTION(BlueprintCallable)
		FVector GetSpawnLocation();

	// Called to set Reset values
	UFUNCTION(BlueprintCallable)
		void Reset();

protected:
	// VARIABLES
	//Game states in order
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_MainGame = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_Loading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_Scanning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_GoalLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_LoadGoal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_GamePlay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_GameOver = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_GameOverButtons = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_SpawnEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_SpawnLocation;
};