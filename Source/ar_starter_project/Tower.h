// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API ATower : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATower();
	//allow access to a component on this object
	UPROPERTY(Category = "Scene Component", VisibleAnywhere, BlueprintReadWrite)USceneComponent* SceneComponent;
	//allow access to a component on this object
	UPROPERTY(Category = "Static Mesh", VisibleAnywhere, BlueprintReadWrite)UStaticMeshComponent* StaticMeshComponent;

	//materials
	UMaterial* StoredMaterial;
	UMaterialInstanceDynamic* DynamicMaterialInst;
	//sound
	USoundBase* DetectionSoundEffect;

	// create trigger capsule
	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* TriggerCapsule;
private:
	FVector StartLocation;
	float t; //delta time
	float LastShot; //delta time
	float timer;
	bool Shooting;
	AActor* TargetToShoot;
	bool DoOnce;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	void SpawnBullets(FVector EnemyLocation);
	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
