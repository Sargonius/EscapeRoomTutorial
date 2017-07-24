// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"



#define OUT
#define LOG true

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();

	SetupInputComponent();
}


void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!InputComponent) { return; }
	if (InputComponent)
	{
		if (LOG) UE_LOG(LogTemp, Warning, TEXT("Input controller found at %s"), *GetOwner()->GetName());

		/// Bind the action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No input controller found at %s"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandler) { return; }
	if (PhysicsHandler)
	{
		// Physics handle is found
		if (LOG) UE_LOG(LogTemp, Warning, TEXT("Physics handle found at %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handler found at %s!"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	

	GetReachLineEnd();

	if (!PhysicsHandler) { return; }
	if (PhysicsHandler->GrabbedComponent)
	{
		PhysicsHandler->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;


	// Get Player view point this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

}

FVector UGrabber::GetReachLineStart()
{

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get Player view point this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));

	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		PhysicsHandler->GrabComponentAtLocation(ComponentToGrab, NAME_None, ActorHit->GetActorLocation());
	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"));
	PhysicsHandler->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{	
	// Line trace out to reach distance
	FHitResult Hit;

	// Setup query parameter
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = Hit.GetActor();
	if (Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *ActorHit->GetName());
	}

	return Hit;
}