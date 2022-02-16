// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPoint.h"
#include "Components/CapsuleComponent.h"
#include "BasicEnemy.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
// Sets default values
AEndPoint::AEndPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	//use constructor helpers to find key parts for the object
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/HandheldARBP/Materials/M_Goal.M_Goal"));
	if (FoundMaterial.Succeeded())
	{
		StoredMaterial = FoundMaterial.Object;
	}
	//create new material 
	DynamicMaterialInst = UMaterialInstanceDynamic::Create(StoredMaterial, StaticMeshComponent);
	//apply the found material
	StaticMeshComponent->SetMaterial(0, DynamicMaterialInst);

	// declare trigger capsule
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(350.0f, 350.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(SceneComponent);

	//set value for enemies already at the goal
	EnemiesAtGoal = 0;
}

// Called when the game starts or when spawned
void AEndPoint::BeginPlay()
{
	Super::BeginPlay();
	// declare overlap events
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AEndPoint::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AEndPoint::OnOverlapEnd);
}
// Called when the game starts or when spawned
void AEndPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//save  start position of cube
	StartLocation = GetActorLocation();
}

void AEndPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Get object of actor hit.
	UClass* hitActorClass = UGameplayStatics::GetObjectClass(OtherActor);

	// if we've hit a enemy.
	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, ABasicEnemy::StaticClass()))
	{
		//increase counter
		EnemiesAtGoal++;
	}
}

void AEndPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Get object of actor hit.
	UClass* hitActorClass = UGameplayStatics::GetObjectClass(OtherActor);

	// if we've hit a enemy.
	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, ABasicEnemy::StaticClass()))
	{
		//EnemiesAtGoal--;		
	}
}

// Called every frame
void AEndPoint::Tick(float DeltaTime)
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
	FScaleMatrix mat_scale(0.125);

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

//return how many enemies have arrived at the goal
int AEndPoint::GetEnemiesAtGoal()
{
	return EnemiesAtGoal;
}


