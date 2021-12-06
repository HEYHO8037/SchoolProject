// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "SchoolPorjectGameMode.generated.h"


UCLASS(minimalapi)
class ASchoolPorjectGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	ASchoolPorjectGameMode();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	void OnUserReady();
	void OnStart();

	void CountDownAdvanceTimer();
	void StartInGameAdvanceTimer();

	int32 GetCountDownTime();
	float GetInGameTime();

	FTimerHandle CountDownTimerHandle;
	FTimerHandle InGameHandle;

private:
	int32 CountDownTime;
	float InGameTimer;
	int32 ReadyCount;

};



