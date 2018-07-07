// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
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

	actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	//Find owning actor
	AActor* owner = GetOwner();
	//Create rotator
	FRotator newRotation = FRotator(0.f, -90.f, 0.f);
	//Set the door rotation
	owner->SetActorRotation(newRotation);

	//FString ownerRotation = owner->GetTransform().GetRotation().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("Door Rotation is at %s!"), *ownerRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll The Trigger Volume
	// If the ActorThatOpens is in the volume 
	if (pressurePlate->IsOverlappingActor(actorThatOpens))
	{
		//Open the door
		OpenDoor();
	}
	
}

