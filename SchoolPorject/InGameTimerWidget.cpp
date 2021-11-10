// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameTimerWidget.h"
#include "SchoolPorjectGameMode.h"
#include "Components/TextBlock.h"


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
		UWorld* World = GetWorld();

		if (World != nullptr)
		{
			ASchoolPorjectGameMode* SPGameMode = Cast<ASchoolPorjectGameMode>(World->GetAuthGameMode());

			if (SPGameMode)
			{
				TimerText->SetText(FText::FromString("00:00"));

				TimerText->TextDelegate.BindUFunction(this, "InGameField");
			}
		}
	}
	return true;
}

FText UInGameTimerWidget::SetTimerField()
{
	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		ASchoolPorjectGameMode* SPGameMode = Cast<ASchoolPorjectGameMode>(World->GetAuthGameMode());

		if (SPGameMode)
		{
			return FText::FromString(FString::FromInt(SPGameMode->GetInGameTime()));
		}
		else
		{
			return FText::FromString("00:00");
		}
	}

	return FText::FromString("00:00");
}
void UInGameTimerWidget::NativeTick(const FGeometry& MyGemotry, float InDeltaTime)
{
	Super::NativeTick(MyGemotry, InDeltaTime);

	TimerText->SetText(SetTimerField());

}

