// Fill out your copyright notice in the Description page of Project Settings.
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameManager::AGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_MainGame = false;

	//use constructor helpers to find key parts for the object
	static ConstructorHelpers::FObjectFinder<USoundBase>USoundWave(TEXT("SoundWave'/Game/HandheldARBP/Audio/327575__ueffects__guitar-background-sample.327575__ueffects__guitar-background-sample'"));
	//set up sound
	StartBGM = USoundWave.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase>USoundWaveAlt(TEXT("SoundWave'/Game/HandheldARBP/Audio/455109__slaking-97__free-music-background-loop-001.455109__slaking-97__free-music-background-loop-001'"));
	//set up sound
	GameBGM = USoundWaveAlt.Object;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
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
	//set game mode value
	m_MainGame = m_DontDestroyOnLoad->GetMainGame();

	//determine which music to play
	if (m_MainGame)
	{
		//play Main bgm
		UGameplayStatics::PlaySoundAtLocation(this, GameBGM, this->GetActorLocation(), 1.0f, 1.0f, 0.0f, (USoundAttenuation*)nullptr, (USoundConcurrency*)nullptr);
	}
	else
	{
		//play start bgm
		UGameplayStatics::PlaySoundAtLocation(this, StartBGM, this->GetActorLocation(), 1.0f, 1.0f, 0.0f, (USoundAttenuation*)nullptr, (USoundConcurrency*)nullptr);
	}
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


