// Copyright Epic Games, Inc. All Rights Reserved.

#include "SchoolPorjectGameMode.h"
#include "SchoolPorjectPawn.h"
#include "SchoolPorjectHud.h"

ASchoolPorjectGameMode::ASchoolPorjectGameMode()
{
	DefaultPawnClass = ASchoolPorjectPawn::StaticClass();
	HUDClass = ASchoolPorjectHud::StaticClass();

	CountDownTime = 3;
	InGameTimer = 0.0f;
}

void ASchoolPorjectGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &ASchoolPorjectGameMode::CountDownAdvanceTimer, 1.0f, true);
}

void ASchoolPorjectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASchoolPorjectGameMode::CountDownAdvanceTimer()
{
    --CountDownTime;

    if (CountDownTime < 1)
    {
        GetWorldTimerManager().ClearTimer(CountDownTimerHandle);
		GetWorldTimerManager().SetTimer(InGameHandle, this, &ASchoolPorjectGameMode::StartInGameAdvanceTimer, 0.1f, true);
		
    }
}

void ASchoolPorjectGameMode::StartInGameAdvanceTimer()
{
	InGameTimer += 0.1f;
}

int32 ASchoolPorjectGameMode::GetCountDownTime()
{
	return CountDownTime;
}

float ASchoolPorjectGameMode::GetInGameTime()
{
	return InGameTimer;
}
