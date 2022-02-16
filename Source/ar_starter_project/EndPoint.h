// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndPoint.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API AEndPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEndPoint();
	//allow access to a component on this object
	UPROPERTY(Category = "Scene Component", VisibleAnywhere, BlueprintReadWrite)USceneComponent* SceneComponent;
	//allow access to a component on this object
	UPROPERTY(Category = "Static Mesh", VisibleAnywhere, BlueprintReadWrite)UStaticMeshComponent* StaticMeshComponent;

	// create trigger capsule
	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* TriggerCapsule;

	//material
	UMaterial* StoredMaterial;
	UMaterialInstanceDynamic* DynamicMaterialInst;
private:
	FVector StartLocation;
	float t; //delta time
	int EnemiesAtGoal;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//return how many enemies have arrived at the goal
	int GetEnemiesAtGoal();
};
