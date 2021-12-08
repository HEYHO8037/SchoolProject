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
}

void ASchoolPorjectGameMode::BeginPlay()
{
	Super::BeginPlay();
}


void ASchoolPorjectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
}
