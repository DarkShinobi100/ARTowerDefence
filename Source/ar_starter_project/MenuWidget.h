// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class AR_STARTER_PROJECT_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool Visible = true;
public:

	// Called to set if the HUD should be displayed values
	UFUNCTION(BlueprintCallable)
		void SetVisible(bool Value);

	// Called to get if the HUD is displayed
	UFUNCTION(BlueprintCallable)
		bool GetVisible();


};
