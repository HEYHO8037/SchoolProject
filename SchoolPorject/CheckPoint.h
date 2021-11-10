// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SchoolPorjectPawn.h"
#include "Components/BoxComponent.h"
#include "CheckPoint.generated.h"

UCLASS()
class SCHOOLPORJECT_API ACheckPoint : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UBoxComponent* CheckPointZone;

	AActor* PlayerPawn;
	ASchoolPorjectPawn* SchoolPorjectPawn;

	bool bIsOnce;

public:	
	// Sets default values for this actor's properties
	ACheckPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
