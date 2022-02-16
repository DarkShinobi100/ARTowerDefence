// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	//use constructor helpers to find key parts for the object
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/HelloAR/Assets/Models/low-poly-camp/source/Camp/Camp.Camp'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	DoOnce = false;
}
// Called when the game starts or when spawned
void ASpawnPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//save  start position of cube
	StartLocation = GetActorLocation();
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!DoOnce)
	{
		DoOnce = true;
		//figure 8 code
		t += DeltaTime;
		float scale = 2 / (3 - cos(2 * t));
		float x = (scale * cos(t)) * 100;
		float y = (scale * sin(2 * t) / 2) * 100;

		//Matrix code
		FMatrix mat_init;

		//set up matrix with start values
		mat_init = FMatrix::Identity;
		mat_init.M[3][0] = StartLocation.X;
		mat_init.M[3][1] = StartLocation.Y;
		mat_init.M[3][2] = StartLocation.Z;

		//resize the actor
		FScaleMatrix mat_scale(0.00230);

		//store final relative position
		FMatrix mat_moving = FMatrix::Identity;
		mat_moving.M[3][0] = x;
		mat_moving.M[3][1] = y;
		mat_moving.M[3][2] = 0.0f;

		//matrix multiplication
		FMatrix mat_final = mat_scale * mat_init;

		//apply final transorm to the cube
		SetActorTransform(FTransform(mat_final));
	}
}


