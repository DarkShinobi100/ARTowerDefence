// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "BulletSphere.h"
#include "Components/CapsuleComponent.h"
#include "BasicEnemy.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
ATower::ATower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//set up components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	//use constructor helpers to find key parts for the object
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/HelloAR/Assets/Models/medieval-tower/source/tower.tower'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<USoundBase>USoundWave(TEXT("SoundWave'/Game/HandheldARBP/Audio/539957__samsterbirdies__sonar-ping.539957__samsterbirdies__sonar-ping'"));
	//set up sound
	DetectionSoundEffect = USoundWave.Object;

	// declare trigger capsule
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(5000.0f, 5000.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	Shooting = false;
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	//// declare overlap events
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ATower::OnOverlapEnd);
	DoOnce = false;
}
// Called when the game starts or when spawned
void ATower::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//save  start position of cube
	StartLocation = GetActorLocation();
}

void ATower::SpawnBullets(FVector EnemyLocation)
{
	//calculate the distance from the enemy
	FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 50.0f);
	FVector VectorResult = EnemyLocation - SpawnLocation;
	VectorResult.Normalize();
	//calculate the rotation and set the bullet
	FRotator SpawnRotation = VectorResult.Rotation();

	//spawn bullet
	FActorSpawnParameters SpawnInfo;
	ABulletSphere* customActor = GetWorld()->SpawnActor<ABulletSphere>(SpawnLocation, SpawnRotation, SpawnInfo);
}

void ATower::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Get object of actor hit.
	UClass* hitActorClass = UGameplayStatics::GetObjectClass(OtherActor);

	// if we've hit a enemy.
	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, ABasicEnemy::StaticClass()))
	{
		//play sound
		UGameplayStatics::PlaySoundAtLocation(this, DetectionSoundEffect, this->GetActorLocation(), 1.0f, 1.0f, 0.0f, (USoundAttenuation*)nullptr, (USoundConcurrency*)nullptr);

		//spawn bullets
		SpawnBullets(OtherActor->GetActorLocation());
		TargetToShoot = OtherActor;
		//begin shooting
		Shooting = true;
	}
}

void ATower::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{//stop shooting
		Shooting = false;
	}
}

// Called every frame
void ATower::Tick(float DeltaTime)
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
		FScaleMatrix mat_scale(0.0125);

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
	//increase timer
	timer += DeltaTime;
	if (Shooting && LastShot + 0.5 > timer)
	{//fire new shot
		LastShot = timer; //reset cooldown
		if (TargetToShoot != nullptr)
		{
			SpawnBullets(TargetToShoot->GetActorLocation());
		}		
	}
}



