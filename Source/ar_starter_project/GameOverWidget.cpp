// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Called to Set if the game over screen is displayed
bool UGameOverWidget::SetGameOverDisplay(bool NewValue)
{
	//Don't Destroy on load
//save a pointer to the game instance then cast it to the desired type
	m_DontDestroyOnLoad = (UDontDestroyOnLoad*)UGameplayStatics::GetGameInstance(GetWorld());
	if (m_DontDestroyOnLoad == nullptr)
	{
		//Error
		return false;
	}
	else
	{
		return m_DontDestroyOnLoad->GetGameOver();
	}
}

//Called to set variables to control visibilty of menu
void UGameOverWidget::SetGameOverButtonDisplay()
{
	m_GameOverDisplay = m_DontDestroyOnLoad->GetGameOver();
	m_ResetButtonsDisplay = m_DontDestroyOnLoad->GetGameOverButton();
}
