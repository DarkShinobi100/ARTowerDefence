// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HUDWidget.h"
#include "GameOverWidget.h"
#include "CustomARPawn.h"
#include "DontDestroyOnLoad.h"
#include "GameManager.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGameManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected:
	//VARIABLES
	 //Don't Destroy on load
	//Variable to store a pointer to the player controlled character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDontDestroyOnLoad* m_DontDestroyOnLoad;

	//variable to set which level we are on
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_MainGame;

	//sounds
	USoundBase* StartBGM;
	USoundBase* GameBGM;
};

