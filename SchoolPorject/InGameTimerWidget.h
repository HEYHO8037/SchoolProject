// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCHOOLPORJECT_API UInGameTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGemotry, float InDeltaTime) override;

protected:
	bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimerText;

	UFUNCTION()
	FText SetTimerField();

};
