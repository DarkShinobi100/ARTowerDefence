// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomActor.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API  ACustomActor : public AActor
{

	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomActor();
	//allow access to a component on this object
	UPROPERTY(Category = "Scene Component", VisibleAnywhere, BlueprintReadWrite)USceneComponent* SceneComponent;
	//allow access to a component on this object
	UPROPERTY(Category = "Static Mesh", VisibleAnywhere, BlueprintReadWrite)UStaticMeshComponent* StaticMeshComponent;

	//materials
	UMaterial* StoredMaterial;
	UMaterialInstanceDynamic* DynamicMaterialInst;

private:
	FVector StartLocation;
	FVector NewLocation;
	float t; //delta time

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
