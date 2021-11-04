// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "HalfCheckPoint.generated.h"

UCLASS()
class SCHOOLPORJECT_API AHalfCheckPoint : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UBoxComponent* HalfCheckPointZone;
public:	
	// Sets default values for this actor's properties
	AHalfCheckPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
