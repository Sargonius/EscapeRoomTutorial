// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing pressure plate at %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll for trigger
	if (GetTotalMassOfActorsOnPlate() >= WeightToOpenDoor)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if (PressurePlate) 
	{  
		PressurePlate->GetOverlappingActors(OverlappingActors);

		for (const auto& Actor : OverlappingActors)
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	return TotalMass;
}

