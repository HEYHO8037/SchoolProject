// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameTimerWidget.h"
#include "SchoolPorjectGameMode.h"
#include "SchoolPorjectPawn.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UInGameTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UInGameTimerWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return false;
	}

	if (TimerText)
	{
		TimerText->SetText(FText::FromString("00:00"));
		TimerText->TextDelegate.BindUFunction(this, "InGameField");
	}

	return true;
}

FText UInGameTimerWidget::SetTimerField()
{
		return FText::FromString(FString::FromInt(Cast<ASchoolPorjectPawn>(GetOwningPlayerPawn())->GetInGameTime()));
}

void UInGameTimerWidget::NativeTick(const FGeometry& MyGemotry, float InDeltaTime)
{
	Super::NativeTick(MyGemotry, InDeltaTime);

	TimerText->SetText(SetTimerField());
}

