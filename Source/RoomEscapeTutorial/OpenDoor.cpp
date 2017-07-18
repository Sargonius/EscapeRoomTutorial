// Fill out your copyright notice in the Description page of Project Settings.

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

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	/*FString Rotation = Owner->GetActorRotation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s rotation is %s"), *Owner->GetName(), *Rotation);
	Rotation = Owner->GetActorRotation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s rotation is %s"), *Owner->GetName(), *Rotation);*/	
}

void UOpenDoor::OpenDoor()
{
	AActor *Owner = GetOwner();
	FRotator DesiredRotation = FRotator(0, OpenAngle, 0);
	Owner->SetActorRotation(DesiredRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll for trigger
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
	}
}

