// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class R3_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, Category = "TEST")
	void CalculateValues();

	UPROPERTY(EditAnywhere, Category = "TEST")
	float RunningTime;

	UPROPERTY(EditAnywhere, Category = "TEST")
	float Multiplier = 100;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "TEST")
	float TestVar;
};
