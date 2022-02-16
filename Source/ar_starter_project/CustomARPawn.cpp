// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomARPawn.h"
#include "CustomActor.h"
#include "PlaneMesh.h"
#include "SpawnPoint.h"
#include "Tower.h"
#include "EndPoint.h"
#include "BasicEnemy.h"
#include "EnemyCube.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ARBlueprintLibrary.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BulletSphere.h"

// Sets default values
ACustomARPawn::ACustomARPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//set timer values
	cameraNotifyLoopTime = 4.0f;
	EnemySpawnNotifyLoopTime = 0.5f;
	Timer = 0.0f; //set counter to zero at start

	//set up components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	//create a camera object
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SceneComponent);
	//use constructor helpers to find key parts for the object
	static ConstructorHelpers::FObjectFinder<UARSessionConfig> ConfigAsset(TEXT("ARSessionConfig'/Game/HandheldARBP/D_ARSessionConfig.D_ARSessionConfig'"));
	Config = ConfigAsset.Object;

	//sound effects
	static ConstructorHelpers::FObjectFinder<USoundBase>USoundWave(TEXT("SoundWave'/Game/HandheldARBP/Audio/150837__toxicwafflezz__bullet-impact-1.150837__toxicwafflezz__bullet-impact-1'"));
	FireSoundEffect = USoundWave.Object;		
	static ConstructorHelpers::FObjectFinder<USoundBase>USoundWaveAlt(TEXT("SoundWave'/Game/HandheldARBP/Audio/539957__samsterbirdies__sonar-ping.539957__samsterbirdies__sonar-ping'"));
	GameOverSoundEffect = USoundWaveAlt.Object;

	//reset counter and toggle values
	MeshToggle = false;
	SpawnPointCounter = 0;
}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();

//Don't Destroy on load
//save a pointer to the game instance then cast it to the desired type
	m_DontDestroyOnLoad = (UDontDestroyOnLoad*)UGameplayStatics::GetGameInstance(GetWorld());
	if (m_DontDestroyOnLoad == nullptr)
	{
		//Error
		return;
	}

	if (m_DontDestroyOnLoad->GetMainGame())
	{
		//start the AR session
		UARBlueprintLibrary::StartARSession(Config);

		//begin camera timers
		GetWorldTimerManager().SetTimer(cameraTicker, this, &ACustomARPawn::DisplayCameraInfo, cameraNotifyLoopTime, true, 0.0f);
		//tell game instance we are loading
		m_DontDestroyOnLoad->SetLoading(true);
		//set control bools for game states
		Start = true;
		PlaneSpawned = false;
		SpawnPointsCounter = 0;
	}
	else
	{
		UARBlueprintLibrary::StopARSession();
	}
}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if we are in the main game
	if (m_DontDestroyOnLoad->GetMainGame())
	{
		Timer += DeltaTime;

		//markers	
		if (Start && Timer >= 2.0f)
		{//disable loading screen
			m_DontDestroyOnLoad->SetLoading(false);
			Start = false;
			//begin scanning
			Scan = true;
		}

		if (Scan)
		{//tell the game instance we are now scanning, to allow UI to update
			m_DontDestroyOnLoad->SetScanning(true);
			//get all images we can track
			AllTrackedImages = UARBlueprintLibrary::GetAllGeometriesByClass<UARTrackedImage>();
			for (int i = 0; i < AllTrackedImages.Num(); i++)
			{
				if (AllTrackedImages[i]->GetDetectedImage())
				{
					//what did we find?
					if (AllTrackedImages[i]->GetDetectedImage()->GetFriendlyName().Equals("VanGogh"))
					{//van gogh
						if (AllObjects.Num() != 0)
						{
							//check spawn point
							for (int J = 0; J < AllObjects.Num(); J++)
							{
								//if it is NOT the same spawn point as last time
								if (ActorCloseEnough(AllTrackedImages[i]->GetLocalToWorldTransform().GetLocation()) && Timer >= TimeSpawned + 2.5)
								{
									//spawn camp
									SpawnObject(AllTrackedImages[i]->GetLocalToWorldTransform().GetLocation(), (FRotator)AllTrackedImages[i]->GetLocalToWorldTransform().GetRotation(), false);
									TimeSpawned = Timer;
								}
							}
						}
						else
						{
							//spawn camp
							SpawnObject(AllTrackedImages[i]->GetLocalToWorldTransform().GetLocation(), (FRotator)AllTrackedImages[i]->GetLocalToWorldTransform().GetRotation(), false);
						}
					}

					if (AllTrackedImages[i]->GetDetectedImage()->GetFriendlyName().Equals("World"))
					{//world					
						if (AllObjects.Num() != 0)
						{
							//check spawn point
							for (int J = 0; J < AllObjects.Num(); J++)
							{
								//if it is NOT the same spawn point as last time
								if (ActorCloseEnough(AllTrackedImages[i]->GetLocalToWorldTransform().GetLocation()) && Timer >= TimeSpawned + 2.5)
								{
									//spawn Tower
									SpawnObject(AllTrackedImages[i]->GetLocalToWorldTransform().GetLocation(), (FRotator)AllTrackedImages[i]->GetLocalToWorldTransform().GetRotation(), true);
									TimeSpawned = Timer;
								}
							}
						}
						else
						{
							//spawn Tower
							SpawnObject(AllTrackedImages[i]->GetLocalToWorldTransform().GetLocation(), (FRotator)AllTrackedImages[i]->GetLocalToWorldTransform().GetRotation(), true);
						}
					}

				}
			}
		}

		//if we are ready to spawn the plane
		if (SpawnPointCounter > 0 && AISetup && !PlaneSpawned)
		{//create it and disable this if
			SpawnPlane();
			PlaneSpawned = true;
		}

		//main game loop
		if (m_DontDestroyOnLoad->GetGamePlay())
		{
			if (EndGoal->GetEnemiesAtGoal() >= 10)
			{
				//gameOver		
				GameOverFunction();
				//reset timer
				Timer = 0.0f;
			}
		}

		//check for gameover
		if (m_DontDestroyOnLoad->GetGameOver() && Timer >= 2.0f)
		{//if it has been on screen for 2.5 seconds, dislay options
			m_DontDestroyOnLoad->SetGameOverButton(true);
		}
	}	
}

// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);
}

// Print current camera orientation as debug message.
void ACustomARPawn::DisplayCameraInfo()
{
	if (Timer > 2.0f)
	{
		// vars for camera location and rotation.
		FVector camLoc;
		FRotator camRot;

		// get camera details
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint(camLoc, camRot);
		// Convert rotation into a vector as camera orientation
		camOri = camRot.Vector();

		FVector camForward = CameraComponent->GetForwardVector() * 100.0f;
	}
}

//spawn a new object
void ACustomARPawn::SpawnObject(FVector SpawnPoint, FRotator SpawnRotation, bool toggle)
{
	FActorSpawnParameters SpawnInfo;

	if (toggle) //spawn tower
	{
		ATower* customActor = GetWorld()->SpawnActor<ATower>(SpawnPoint, SpawnRotation, SpawnInfo);
		//add the new object to this array
		AllObjects.Add(customActor);
	}
	else //spawn camp
	{
		ASpawnPoint* customActor = GetWorld()->SpawnActor<ASpawnPoint>(SpawnPoint, SpawnRotation, SpawnInfo);
		//add the new object to this array
		AllObjects.Add(customActor);
		SpawnPoints.Add(customActor); //add this new oject to our array of spawn locations
		SpawnPointCounter++;
	}
}


// binding touch events
void ACustomARPawn::OnScreenTouch(const ETouchIndex::Type fingerIndex, const FVector screenPos)
{	
	//check if in main game
	if (m_DontDestroyOnLoad->GetMainGame())
	{
		//check if we are setting the goal up
		if (AISetup && PlaneSpawned && m_DontDestroyOnLoad->GetLoadGoal())
		{
			// Perform a hitTest, get the return values as hitTesTResult
			if (!WorldHitTest(FVector2D(screenPos), hitTestResult))
			{// HitTest returned false, get out.
				return;
			}
			else
			{
				// Get object of actor hit.
				UClass* hitActorClass = UGameplayStatics::GetObjectClass(hitTestResult.GetActor());

				// if we've hit a plane.
				if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, APlaneMesh::StaticClass()))
				{
					//spawn the goal where the player tapped
					SetGoal(hitTestResult.Location);
				}
			}
		}

		//check for if we are in the main game
		if (m_DontDestroyOnLoad->GetGamePlay())
		{
			// Perform a hitTest, get the return values as hitTesTResult
			if (!WorldHitTest(FVector2D(screenPos), hitTestResult))
			{// HitTest returned false, get out.
				return;
			}
			else
			{
				// Get object of actor hit.
				UClass* hitActorClass = UGameplayStatics::GetObjectClass(hitTestResult.GetActor());

				// if we've hit the plane.
				if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, APlaneMesh::StaticClass()) && Timer >= TimeSpawned + 0.25)
				{//spawn a wall where the player tapped
					SpawnWall(hitTestResult.Location);
					//reset cooldown
					TimeSpawned = Timer;
				}

				// if we've hit an enemy.
				if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, ABasicEnemy::StaticClass()))
				{
					//shoot at them
					SpawnBullets(hitTestResult.Location);
				}
			}
		}
	}	
}

bool ACustomARPawn::WorldHitTest(FVector2D screenPos, FHitResult& hitResult)
{
	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	FVector worldPosition;
	FVector worldDirection;
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenPos, /*out*/worldPosition, /*out*/worldDirection);

	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceVector = worldDirection * 1000.0;
	traceVector = worldPosition + traceVector;

	// perform line trace
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(hitResult, worldPosition, traceVector, ECollisionChannel::ECC_WorldDynamic);

	// return if the operation was successful
	return traceSuccess;
}

void ACustomARPawn::SetAISetup()
{//check there are spawn point already
	if (SpawnPointCounter != 0)
	{//disable scanning mode
		Scan = false;
		//enable AI set up
		AISetup = true;

		//update game instance to update UI
		m_DontDestroyOnLoad->SetScanning(false);
		m_DontDestroyOnLoad->SetLoadGoal(true);
	}
}

void ACustomARPawn::LoadGame()
{
	m_DontDestroyOnLoad->SetMainGame(true);
	FName LevelToLoad = FName("MainLevel");
	UGameplayStatics::OpenLevel(this, LevelToLoad);
}

void ACustomARPawn::ResetGame()
{
	m_DontDestroyOnLoad->SetMainGame(false);
	FName LevelToLoad = FName("StartMenu");
	UGameplayStatics::OpenLevel(this, LevelToLoad);
}

void ACustomARPawn::QuitGame()
{
	FGenericPlatformMisc::RequestExit(true);
}

bool ACustomARPawn::ActorCloseEnough(FVector SpawnPoint)
{
	for (int i = 0; i < AllObjects.Num(); i++)
	{//is the actor we are trying to spawn in the same location as anothers X(give or take +- 2)
		if (SpawnPoint.X <= AllObjects[i]->GetActorLocation().X + 2 && SpawnPoint.X >= AllObjects[i]->GetActorLocation().X - 2)
		{
			return false;
		}//is the actor we are trying to spawn in the same location as anothers Y(give or take +- 2)
		else if (SpawnPoint.Y <= AllObjects[i]->GetActorLocation().Y + 2 && SpawnPoint.Y >= AllObjects[i]->GetActorLocation().Y - 2)
		{
			return false;
		}//is the actor we are trying to spawn in the same location as anothers Z(give or take +- 2)
		else if (SpawnPoint.Z <= AllObjects[i]->GetActorLocation().Z + 2 && SpawnPoint.Z >= AllObjects[i]->GetActorLocation().Z - 2)
		{
			return false;
		}
	}
	//if not then we can spawn them
	return true;
}

void ACustomARPawn::SpawnPlane()
{
	//find average z coordinate for height
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;

	for (int i = 0; i < AllObjects.Num(); i++)
	{
		X += AllObjects[i]->GetActorLocation().X;
		Y += AllObjects[i]->GetActorLocation().Y;
		Z += AllObjects[i]->GetActorLocation().Z;
	}
	//centre of all markers
	X = X / AllObjects.Num();
	Y = Y / AllObjects.Num();
	Z = Z / AllObjects.Num();
	Z -= 100.0f;

	FVector Length;

	float LongestX;
	float LongestY;
	for (int i = 0; i < AllObjects.Num(); i++)
	{
		//find distance between markers
		if (i == 0) //if i == 0 set to 0
		{
			LongestX = 0.0f;
			LongestY = 0.0f;
		}
		else //compare this point to the point before it in the array
		{
			Length = AllObjects[i]->GetActorLocation() - AllObjects[i - 1]->GetActorLocation();
		}

		//if the distance in X is bigger than current record
		if (Length.X > LongestX)
		{//save it
			LongestX = Length.X;
		}

		//if distance in Y is bigger than current record
		if (Length.Y > LongestY)
		{//save it
			LongestY = Length.Y;
		}
	}

	//spawn new plane mesh at centre of markers
	FActorSpawnParameters SpawnInfo;
	FVector SpawnPoint = FVector(X, Y, Z);
	FRotator SpawnRotation = FRotator(0, 0, 0);

	APlaneMesh* customActor = GetWorld()->SpawnActor<APlaneMesh>(SpawnPoint, SpawnRotation, SpawnInfo);
	//set size of mesh
	FVector scale = FVector(10, 10, 1);
	customActor->SetActorScale3D(scale);
}

void ACustomARPawn::SetGoal(FVector SpawnPoint)
{//spawn at location we recive
	FVector SpawnLocation = FVector(SpawnPoint.X, SpawnPoint.Y, SpawnPoint.Z);
	FRotator SpawnRotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;
		
	//spawn the goal
	AEndPoint* customActor = GetWorld()->SpawnActor<AEndPoint>(SpawnLocation, SpawnRotation, SpawnInfo);
	//save a pointer to it
	EndGoal = customActor;

	//update game state
	AISetup = false;
	Gameplay = true;

	//update values in game instance
	m_DontDestroyOnLoad->SetGoalLocation(SpawnLocation);

	//update game instance values to update UI
	m_DontDestroyOnLoad->SetLoadGoal(false);
	m_DontDestroyOnLoad->SetGamePlay(true);

	//try spawning enemies once every 5 seconds
	GetWorldTimerManager().SetTimer(cameraTicker, this, &ACustomARPawn::SpawnEnemiesAtRandomLocation, EnemySpawnNotifyLoopTime, true, 0.0f);
	TimeSpawned = Timer;
}

void ACustomARPawn::SpawnEnemiesAtRandomLocation()
{//check if it is time to spawn
	if (!m_DontDestroyOnLoad->GetGameOver() && Timer >= EnemyTimeSpawned + 2.5f)
	{
		//reset the cooldown
		EnemyTimeSpawned = Timer;
		if (SpawnPointsCounter > 1)//is there more than 1 spawn point
		{
			//alternate spawn loactions
			if (SpawnPointsCounter == 0)
			{
				SpawnPointsCounter = 1;
			}
			else
			{
				SpawnPointsCounter = 0;
			}
			//spawn at designated location
			FVector SpawnLocation = SpawnPoints[SpawnPointsCounter]->GetActorLocation();

			//offset the spawn slightly to avoid collisions
			SpawnLocation.Y += 50;
			SpawnLocation.Z = m_DontDestroyOnLoad->GetGoalLocation().Z +20;

			//save values to game instance to allow BP to spawn enemy
			m_DontDestroyOnLoad->SetSpawnLocation(SpawnLocation);
			m_DontDestroyOnLoad->SetSpawnEnemy(true);
		}
		else //only 1 spawn point
		{
			//spawn at default position
			FVector SpawnLocation = SpawnPoints[0]->GetActorLocation();
			SpawnLocation.Y += 50;
			SpawnLocation.Z = m_DontDestroyOnLoad->GetGoalLocation().Z +10;
			//save values to game instance to allow BP to spawn enemy
			m_DontDestroyOnLoad->SetSpawnEnemy(true);
			m_DontDestroyOnLoad->SetSpawnLocation(SpawnLocation);
		}		
	}
	else
	{//can't spawn an enemy at the moment
		m_DontDestroyOnLoad->SetSpawnEnemy(false);
	}
}

void ACustomARPawn::GameOverFunction()
{
	//play sound effect
	UGameplayStatics::PlaySoundAtLocation(this, FireSoundEffect, camOri, 1.0f, 1.0f, 0.0f, (USoundAttenuation*)nullptr, (USoundConcurrency*)nullptr);
	//reset game values
	Start = false;
	Scan = false;
	AISetup = false;
	PlaneSpawned = false;
	Gameplay = false;
	//set game over to true
	GameOver = true;

	//update game instance to update UI
	m_DontDestroyOnLoad->SetLoading(false);
	m_DontDestroyOnLoad->SetScanning(false);
	m_DontDestroyOnLoad->SetLoadGoal(false);
	m_DontDestroyOnLoad->SetGamePlay(false);
	m_DontDestroyOnLoad->SetGameOver(true);
}

void ACustomARPawn::SpawnWall(FVector SpawnPoint)
{//spawn a cube where the player taps
	FVector SpawnLocation = FVector(SpawnPoint.X, SpawnPoint.Y, SpawnPoint.Z);
	FRotator SpawnRotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;
	AEnemyCube* customActor = GetWorld()->SpawnActor<AEnemyCube>(SpawnLocation, SpawnRotation, SpawnInfo);
}

void ACustomARPawn::SpawnBullets(FVector EnemyLocation)
{
	//calculate the distance from the enemy
	FVector SpawnLocation = FVector(camOri.X, camOri.Y, camOri.Z + 50.0f);
	FVector VectorResult = EnemyLocation - SpawnLocation;
	VectorResult.Normalize();
	//calculate the rotation and set the bullet
	FRotator SpawnRotation = VectorResult.Rotation();

	//spawn bullet
	FActorSpawnParameters SpawnInfo;
	ABulletSphere* customActor = GetWorld()->SpawnActor<ABulletSphere>(SpawnLocation, SpawnRotation, SpawnInfo);

	//play sound effect
	UGameplayStatics::PlaySoundAtLocation(this, FireSoundEffect, SpawnLocation, 1.0f, 1.0f, 0.0f, (USoundAttenuation*)nullptr, (USoundConcurrency*)nullptr);
}