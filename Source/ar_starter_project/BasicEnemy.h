// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "dontdestroyonload.h"
#include "BasicEnemy.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void ReceiveDamage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to play sound effect at location
	UFUNCTION(BlueprintCallable)
		void UpdateGoal();

	// Called to play sound effect at location
	UFUNCTION(BlueprintCallable)
		void PlaySoundEffect(USoundBase* soundEffect);

protected:
	//Don't Destroy on load
	//Variable to store a pointer to the player controlled character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDontDestroyOnLoad* m_DontDestroyOnLoad;
	//variable to store target to move towards
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_Target;
};
