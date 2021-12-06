// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownWidget.h"
#include "SchoolPorjectGameMode.h"
#include "Components/TextBlock.h"


void UCountDownWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UCountDownWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return false;
	}

	if (CountDownText)
	{
		UWorld* World = GetWorld();

		if (World != nullptr)
		{
			ASchoolPorjectGameMode* SPGameMode = Cast<ASchoolPorjectGameMode>(World->GetAuthGameMode());

			if (SPGameMode)
			{
				CountDownText->SetText(FText::FromString("Ready"));

				CountDownText->TextDelegate.BindUFunction(this, "CountDownField");
			}
		}
	}
	return true;
}

FText UCountDownWidget::SetNumberField()
{
	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		ASchoolPorjectGameMode* SPGameMode = Cast<ASchoolPorjectGameMode>(World->GetAuthGameMode());

		if (SPGameMode)
		{
			if (SPGameMode->GetCountDownTime() == 0)
			{
				RemoveFromParent();
			}

			return FText::FromString(FString::FromInt(SPGameMode->GetCountDownTime()));
		}
		else
		{
			return FText::FromString("Ready");
		}
	}

	return FText::FromString("Ready");
}
void UCountDownWidget::NativeTick(const FGeometry& MyGemotry, float InDeltaTime)
{
	Super::NativeTick(MyGemotry, InDeltaTime);

	//CountDownText->SetText(SetNumberField());

 }
