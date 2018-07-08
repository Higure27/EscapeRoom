// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber Reporting Start"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get player view point this tick
	APlayerController* mainPlayer = GetWorld()->GetFirstPlayerController();
	FVector  playerLocation;
	FRotator playerRotation; 
	mainPlayer->GetPlayerViewPoint(OUT playerLocation, OUT playerRotation);

	//TODO log out every tick 
	UE_LOG(LogTemp, Warning, TEXT("PlayerLocation %s, Player Rotation %s")
		,*playerLocation.ToString(),*playerRotation.ToString());

	//Ray-cast out to reach distance 

	//See what we hit 
}

