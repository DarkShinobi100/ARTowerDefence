// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCube.h"
#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemyCube::AEnemyCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cube"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	//use constructor helpers to find key parts for the object
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/HandheldARBP/Materials/M_BackgroundFade.M_BackgroundFade"));
	if (FoundMaterial.Succeeded())
	{
		StoredMaterial = FoundMaterial.Object;
	}
	//create new material 
	DynamicMaterialInst = UMaterialInstanceDynamic::Create(StoredMaterial, StaticMeshComponent);
	//apply the found material
	StaticMeshComponent->SetMaterial(0, DynamicMaterialInst);

	// declare trigger capsule
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetBoxExtent(FVector(32.0f,32.0f,32.0f));
	Collider->SetCollisionProfileName(TEXT("WorldStatic"));
	Collider->SetupAttachment(RootComponent);

	//reset timer
	t = 0.0f;
}


// Called when the game starts or when spawned
void AEnemyCube::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the game starts or when spawned
void AEnemyCube::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//save  start position of cube
	StartLocation = GetActorLocation();
}

// Called every frame
void AEnemyCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	FScaleMatrix mat_scale(0.50);

	//store final relative position
	FMatrix mat_moving = FMatrix::Identity;
	mat_moving.M[3][0] = x;
	mat_moving.M[3][1] = y;
	mat_moving.M[3][2] = StartLocation.Z;


	//matrix multiplication
	FMatrix mat_final = mat_scale * mat_init;

	//apply final transorm to the cube
	SetActorTransform(FTransform(mat_final));

	if (t >= 2.5f)
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
}

