// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPETUTORIAL_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetReachLineEnd();
	FVector GetReachLineStart();
	const FHitResult GetFirstPhysicsBodyInReach();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
private:
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandler = nullptr;
	UInputComponent* InputComponent = nullptr;

	FVector LineTraceEnd;

	void SetupInputComponent();
	void FindPhysicsHandleComponent();
	void Grab();
	void Release();	
};
