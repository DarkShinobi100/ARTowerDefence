// Fill out your copyright notice in the Description page of Project Settings.
#include "HUDWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UHUDWidget::UpdateUI()
{
	//Don't Destroy on load
	//save a pointer to the game instance then cast it to the desired type
	m_DontDestroyOnLoad = (UDontDestroyOnLoad*)UGameplayStatics::GetGameInstance(GetWorld());
	if (m_DontDestroyOnLoad == nullptr)
	{
		//Error
		return;
	}
	else
	{//set variables to control visibilty of menu
		m_DisplayLoading =  m_DontDestroyOnLoad->GetLoading();
		m_DisplayScanning = m_DontDestroyOnLoad->GetScanning();
		m_DisplayGoal = m_DontDestroyOnLoad->GetLoadGoal();
		m_DisplayHUD = m_DontDestroyOnLoad->GetGamePlay();
	}
}
