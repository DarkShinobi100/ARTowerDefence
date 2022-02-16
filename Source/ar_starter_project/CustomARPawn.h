// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include"Camera/CameraComponent.h"
#include "Engine/EngineTypes.h"
#include "ARBlueprintLibrary.h"
#include "DontDestroyOnLoad.h"
#include "EndPoint.h"
#include "CustomARPawn.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomARPawn();
	//allow access to a component on this object
	UPROPERTY(Category = "Scene Component", VisibleAnywhere, BlueprintReadWrite)USceneComponent* SceneComponent;
	//allow access to a component on this object
	UPROPERTY(Category = "Camera component", VisibleAnywhere, BlueprintReadWrite)UCameraComponent* CameraComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	//camera variables
	FTimerHandle cameraTicker; 
	float cameraNotifyLoopTime;
	FVector camOri;

	//enemy variables
	float EnemySpawnNotifyLoopTime;

	//timer
	float Timer;
	//save hit result
	FHitResult hitTestResult;
	//value to control spawns
	bool MeshToggle;
	int SpawnPointCounter;

	//tracking info
	UARSessionConfig* Config;
	TArray<UARTrackedImage*> AllTrackedImages;

	//array of marker objects 
	TArray<AActor*> AllObjects;
	//array of spawn point locations
	TArray<AActor*> SpawnPoints;

	int SpawnPointsCounter;
	float Alpha;

	//line trace value
	TArray<FARTraceResult> LineTraceObjects;

	//sound effects
	USoundBase* FireSoundEffect;
	USoundBase* GameOverSoundEffect;

	//end goal actor
	AEndPoint* EndGoal;

	//Don't Destroy on load
   //Variable to store a pointer to the player controlled character
	UDontDestroyOnLoad* m_DontDestroyOnLoad;

	//game state controllers
	bool Start = true;
	bool Scan = false;
	bool AISetup = false;
	bool PlaneSpawned = false;
	bool Gameplay = false;
	bool GameOver = false;
	//timers for cooldowns
	float TimeSpawned = 0.0f;
	float EnemyTimeSpawned = 0.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//update camera info
	virtual void DisplayCameraInfo();
	//spawn an object(false = camp, true = tower)
	void SpawnObject(FVector SpawnPoint, FRotator SpawnRotation, bool toggle);
	//function to test for touches
	virtual void OnScreenTouch(const ETouchIndex::Type fingerIndex, const FVector screenPos);
	//function to check for hits in scene
	bool WorldHitTest(FVector2D screenPos, FHitResult& hitTestResult);

	//function to allow AI to be setup
	UFUNCTION(BlueprintCallable)
	void SetAISetup();

	//function to load the game
	UFUNCTION(BlueprintCallable)
		void LoadGame();

	//function to reload the start level
	UFUNCTION(BlueprintCallable)
		void ResetGame();

	//function called to quit the game
	UFUNCTION(BlueprintCallable)
		void QuitGame();
private:
	//check actor is close enough
	bool ActorCloseEnough(FVector SpawnPoint);
	
	//funtion to spawn plane in centre of marker objects
	void SpawnPlane();

	//function to spawn the end goal
	void SetGoal(FVector SpawnPoint);
	
	//function to spawn enemies at random spawn points from the markers
	void SpawnEnemiesAtRandomLocation();

	//function to reset values for gameover
	void GameOverFunction();

	//function to create wall at tapped location
	void SpawnWall(FVector SpawnPoint);

	//function to create bullets 
	void SpawnBullets(FVector EnemyLocation);
};
