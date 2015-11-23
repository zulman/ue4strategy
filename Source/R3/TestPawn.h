// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TestPawn.generated.h"

UCLASS()
class R3_API ATestPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATestPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* OurVisibleComponent;

	////Input functions
	//void Move_XAxis(float AxisValue);
	//void Move_YAxis(float AxisValue);
	//void StartGrowing();
	//void StopGrowing();

	////Input variables
	//FVector CurrentVelocity;
	//bool bGrowing;
	
	//Input variables
	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;

	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ZoomIn();
	void ZoomOut();

protected:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* OurCameraSpringArm;

	UCameraComponent* OurCamera;
};
