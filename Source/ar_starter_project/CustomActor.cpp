// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomActor.h"
#include"Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"

// Sets default values
ACustomActor::ACustomActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cube"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/HandheldARBP/Materials/M_DefautMaterial.M_DefautMaterial"));
	if (FoundMaterial.Succeeded())
	{
		StoredMaterial = FoundMaterial.Object;
	}
	DynamicMaterialInst = UMaterialInstanceDynamic::Create(StoredMaterial, StaticMeshComponent);

	StaticMeshComponent->SetMaterial(0, DynamicMaterialInst);

	StaticMeshComponent->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
	StaticMeshComponent->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));

	t = 0.0f;

}

// Called when the game starts or when spawned
void ACustomActor::BeginPlay()
{
	Super::BeginPlay();
	UKismetSystemLibrary::PrintString(this, FString(GetActorLocation().ToString()), true, true, FLinearColor(0, 1, 1, 1), 5);

}

// Called when the game starts or when spawned
void ACustomActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//save  start position of cube
	StartLocation = GetActorLocation();
}

// Called every frame
void ACustomActor::Tick(float DeltaTime)
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

	//store final relative position
	FMatrix mat_moving = FMatrix::Identity;
	mat_moving.M[3][0] = x;
	mat_moving.M[3][1] = y;
	mat_moving.M[3][2] = StartLocation.Z;


	//matrix multiplication
	FMatrix mat_final = mat_moving * mat_init;

	//apply final transorm to the cube
	SetActorTransform(FTransform(mat_final));
}



