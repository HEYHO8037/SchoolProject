// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCHOOLPORJECT_API UReadyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ToggleReady();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReadyButton;

};
