// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DontDestroyOnLoad.h"
#include "GameOverWidget.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Called to Set if the game over screen is displayed
	UFUNCTION(BlueprintCallable)
		bool SetGameOverDisplay(bool NewValue);

	//Called to set variables to control visibilty of menu
	UFUNCTION(BlueprintCallable)
		void SetGameOverButtonDisplay();

protected:
	//VARIABLES
	 //Don't Destroy on load
	//Variable to store a pointer to the player controlled character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDontDestroyOnLoad* m_DontDestroyOnLoad;
	
	//booleans
	// Bool to control when to display Game over image
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_GameOverDisplay;
	// Bool to control when to display menu buttons
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_ResetButtonsDisplay;
};
