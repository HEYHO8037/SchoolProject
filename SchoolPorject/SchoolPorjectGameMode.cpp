// Copyright Epic Games, Inc. All Rights Reserved.

#include "SchoolPorjectGameMode.h"
#include "SchoolPorjectPawn.h"
#include "SchoolPorjectHud.h"
#include "Kismet/GameplayStatics.h"

ASchoolPorjectGameMode::ASchoolPorjectGameMode()
{
	DefaultPawnClass = ASchoolPorjectPawn::StaticClass();
	HUDClass = ASchoolPorjectHud::StaticClass();

	ReadyCount = 0;
	CountDownTime = 3;
	InGameTimer = 0.0f;
}

void ASchoolPorjectGameMode::BeginPlay()
{
	Super::BeginPlay();
}


void ASchoolPorjectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASchoolPorjectGameMode::CountDownAdvanceTimer()
{
    --CountDownTime;
	UE_LOG(LogTemp, Warning, TEXT("0"));

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

void ASchoolPorjectGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	TSubclassOf<ASchoolPorjectPawn> player;
	TArray<AActor*> getPlayer;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), player, getPlayer);
	
	for (AActor* Actor : getPlayer)
	{
		ASchoolPorjectPawn* pawn = Cast<ASchoolPorjectPawn>(Actor);
		pawn->OnUserCountChanged(2);
	}
}

void ASchoolPorjectGameMode::OnUserReady()
{
	ReadyCount++;

	if (ReadyCount >= 2)
	{
		TSubclassOf<ASchoolPorjectPawn> player;
		player = ASchoolPorjectPawn::StaticClass();
		TArray<AActor*> getPlayer;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), player, getPlayer);
		for (AActor* Actor : getPlayer)
		{
			ASchoolPorjectPawn* pawn = Cast<ASchoolPorjectPawn>(Actor);
			pawn->OnStart();
			OnStart();
		}
	}
}

void ASchoolPorjectGameMode::OnStart()
{
	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &ASchoolPorjectGameMode::CountDownAdvanceTimer, 1.0f, true);
}
