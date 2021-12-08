// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownWidget.h"
#include "SchoolPorjectPawn.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


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
		CountDownText->SetText(FText::FromString("Ready"));
		CountDownText->TextDelegate.BindUFunction(this, "CountDownField");
	}
	return true;
}

FText UCountDownWidget::SetNumberField()
{
	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		TSubclassOf<ASchoolPorjectPawn> player;
		player = ASchoolPorjectPawn::StaticClass();
		TArray<AActor*> getPlayer;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), player, getPlayer);
		for (AActor* Actor : getPlayer)
		{
			ASchoolPorjectPawn* pawn = Cast<ASchoolPorjectPawn>(Actor);
			if (pawn->GetCountDownTime() == 0)
			{
				RemoveFromParent();
			}
		}
		
		return FText::FromString(FString::FromInt(Cast<ASchoolPorjectPawn>(GetOwningPlayerPawn())->GetCountDownTime()));
	}
	else
	{
		return FText::FromString("Ready");
	}

	return FText::FromString("Ready");
}

void UCountDownWidget::NativeTick(const FGeometry& MyGemotry, float InDeltaTime)
{
	Super::NativeTick(MyGemotry, InDeltaTime);
	CountDownText->SetText(SetNumberField());

 }
