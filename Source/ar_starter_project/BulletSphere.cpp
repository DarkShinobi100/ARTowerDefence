// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletSphere.h"
#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "BasicEnemy.h"
#include "Math/Vector.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
ABulletSphere::ABulletSphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set up components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	//use constructor finders to find key elements of the object
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/HandheldARBP/Materials/M_Spawn.M_Spawn"));
	if (FoundMaterial.Succeeded())
	{
		StoredMaterial = FoundMaterial.Object;
	}
	DynamicMaterialInst = UMaterialInstanceDynamic::Create(StoredMaterial, StaticMeshComponent);

	//set the material to the located one
	StaticMeshComponent->SetMaterial(0, DynamicMaterialInst);

	// declare trigger capsule
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(SceneComponent);

	//reset timer value
	t = 0.0f;
}


// Called when the game starts or when spawned
void ABulletSphere::BeginPlay()
{
	Super::BeginPlay();

	// declare overlap events
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABulletSphere::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ABulletSphere::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ABulletSphere::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABulletSphere::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{//don't do collisiion reactions in the first 0.25 seconds
	if (t >= 0.25f)
	{ //If we hit anything
		ReceiveDamage();
	}	
}

void ABulletSphere::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (t >= 0.25f)
	{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			//ended the overlap, doesn't do anything but collisions work best as pairs
		}
	}

}

// Called every frame
void ABulletSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//increment time value
	t += DeltaTime;

	//resize actor	
	this->SetActorScale3D(FVector(0.01f, 0.01f, 0.01f));

	//movement code
	FVector Location = GetActorLocation();	
	float CurrentVelocity = (Speed * DeltaTime) * AccelerationRate;
	Location += GetActorForwardVector() * CurrentVelocity;

	//set actor location after moving forward based on our rotation, allows bullet to move correctly
	SetActorLocation(Location);
}

void ABulletSphere::ReceiveDamage()
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

