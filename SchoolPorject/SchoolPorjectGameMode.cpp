// Copyright Epic Games, Inc. All Rights Reserved.

#include "SchoolPorjectGameMode.h"
#include "SchoolPorjectPawn.h"
#include "SchoolPorjectHud.h"

ASchoolPorjectGameMode::ASchoolPorjectGameMode()
{
	DefaultPawnClass = ASchoolPorjectPawn::StaticClass();
	HUDClass = ASchoolPorjectHud::StaticClass();

	CountDownTime = 3;
}

void ASchoolPorjectGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ASchoolPorjectGameMode::AdvanceTimer, 1.0f, true);
}

void ASchoolPorjectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASchoolPorjectGameMode::AdvanceTimer()
{
    --CountDownTime;

    if (CountDownTime < 1)
    {
        GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("222"));
    }
}

int32 ASchoolPorjectGameMode::GetCountDownTime()
{
	return CountDownTime;
}
