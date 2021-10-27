// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "SchoolPorjectHud.generated.h"


UCLASS(config = Game)
class ASchoolPorjectHud : public AHUD
{
	GENERATED_BODY()

public:
	ASchoolPorjectHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
