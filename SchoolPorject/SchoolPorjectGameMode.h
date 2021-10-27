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
	void AdvanceTimer();
	int32 GetCountDownTime();

	FTimerHandle CountdownTimerHandle;


private:
	int32 CountDownTime;
};



