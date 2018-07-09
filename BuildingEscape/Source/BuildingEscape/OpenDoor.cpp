// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

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

	//Find owning actor
	owner = GetOwner();

	if (!pressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("No associated pressure plate found on %s"), *owner->GetName());
	}

}


void UOpenDoor::CloseDoor()
{
	//Set the door rotation
	owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));

	//FString ownerRotation = owner->GetTransform().GetRotation().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("Door Rotation is at %s!"), *ownerRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll The Trigger Volume
	if (TotalMassOfActorsOnPlate()>= triggerRequiredMass)
	{
		//Open the door
		onOpen.Broadcast();
	}
	else
	//check if it is time to close the door 
	//if (GetWorld()->GetTimeSeconds() -lastDoorOpenTime > doorCloseDelay)
	{
		//Close Door
		onClose.Broadcast();
	}
	
}

float UOpenDoor::TotalMassOfActorsOnPlate() const
{
	float totalMass = 0.f;
	TArray<AActor*> overlappingActors;

	if (!pressurePlate){	return -1.f;}
	//Find all the overlapping actors
	pressurePlate->GetOverlappingActors(OUT overlappingActors);
	//Iterate through them adding their masses
	for (const auto* currentActor : overlappingActors)
	{
		totalMass += currentActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("On platform is actor %s"), *currentActor->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("mass is currently %s"), *(FString::SanitizeFloat(totalMass)));
	}

	return totalMass;
}

