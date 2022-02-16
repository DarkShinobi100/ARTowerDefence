// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemy.h"
#include "Components/CapsuleComponent.h"
#include "BulletSphere.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ABasicEnemy::ABasicEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicEnemy::ReceiveDamage()
{//kill self
	if (!this->IsPendingKill())
	{//As delete results in nullpointers quite often
		//delete(this);
		//Instead we hide the actor, disable collisions
		this->SetActorEnableCollision(false);
		this->SetActorHiddenInGame(true);
		this->SetActorLocation(FVector(1000, 1000, -1000));
		//and have the engine itself clean it up for us
		this->MarkPendingKill();
	}
}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABasicEnemy::UpdateGoal()
{
	//Don't Destroy on load
	//save a pointer to the game instance then cast it to the desired type
	m_DontDestroyOnLoad = (UDontDestroyOnLoad*)UGameplayStatics::GetGameInstance(GetWorld());
	if (m_DontDestroyOnLoad == nullptr)
	{
		//Error
		return;
	}
	else
	{
		//set the AI target to the goals location
		m_Target = FVector(m_DontDestroyOnLoad->GetGoalLocation().X, m_DontDestroyOnLoad->GetGoalLocation().Y, this->GetActorLocation().Z);
	}
}

void ABasicEnemy::PlaySoundEffect(USoundBase* soundEffect)
{
	// Play the desired sound effect
	UGameplayStatics::PlaySoundAtLocation(this, soundEffect, this->GetActorLocation(), 1.0f, 1.0f, 0.0f, (USoundAttenuation*)nullptr, (USoundConcurrency*)nullptr);
}

