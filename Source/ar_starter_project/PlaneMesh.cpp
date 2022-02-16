// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneMesh.h"
#include "Components/BoxComponent.h"

// Sets default values
APlaneMesh::APlaneMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	//use constructor helpers to find key parts for the object
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/HandheldARBP/Materials/M_DefautMaterial.M_DefautMaterial"));
	if (FoundMaterial.Succeeded())
	{
		StoredMaterial = FoundMaterial.Object;
	}
	//create new material 
	DynamicMaterialInst = UMaterialInstanceDynamic::Create(StoredMaterial, StaticMeshComponent);
	//apply the found material
	StaticMeshComponent->SetMaterial(0, DynamicMaterialInst);
}

// Called when the game starts or when spawned
void APlaneMesh::BeginPlay()
{
	Super::BeginPlay();	
}
// Called when the game starts or when spawned
void APlaneMesh::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void APlaneMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


