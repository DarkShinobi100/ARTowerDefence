// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DontDestroyOnLoad.h"
#include "HUDWidget.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Called to update variable values
	UFUNCTION(BlueprintCallable)
		void UpdateUI();
	
protected:
	//VARIABLES
	 //Don't Destroy on load
	//Variable to store a pointer to the player controlled character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDontDestroyOnLoad* m_DontDestroyOnLoad;

	//booleans
	// Bool to control when to display loading screen
	 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_DisplayLoading = false;;
	// Bool to control when to display scanning info
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_DisplayScanning = false;;

	// Bool to control when to display goal info
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_DisplayGoal = false;;

	// Bool to control when to display main HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_DisplayHUD = false;
};