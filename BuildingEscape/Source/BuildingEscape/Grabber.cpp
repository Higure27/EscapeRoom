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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComp();
	
	SetupInputComponent();

}

///Look for attahched physics handle
void UGrabber::FindPhysicsHandleComp()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (physicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find %s physics handle component"), *GetOwner()->GetName())
	}
}

///Look for attached input component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		///Bind the input action
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find %s input component"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));

	//Line trace and see if we reach any actors with physics body collision channel set
	auto hitResult = GetFirstPhysicsBodyInReach();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();
	/// if we hit something 
	if (componentToGrab)
	{
		if (actorHit)
		{
			if (!physicsHandle) { return; }
			//Attach physics handle
			physicsHandle->GrabComponentAtLocationWithRotation(
				componentToGrab,
				NAME_None,
				actorHit->GetActorLocation(),
				actorHit->GetActorRotation()
			);
		}
	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	//TODO release physics handle 
	physicsHandle->ReleaseComponent();
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

	FVector lineTraceEnd = playerLocation + playerRotation.Vector()*reach;


	//if the physics handle is attached 
	if (physicsHandle->GrabbedComponent)
	{
		//move the object that is attached
		physicsHandle->SetTargetLocation(lineTraceEnd);
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//Get player view point this tick
	APlayerController* mainPlayer = GetWorld()->GetFirstPlayerController();
	FVector  playerLocation;
	FRotator playerRotation;
	mainPlayer->GetPlayerViewPoint(OUT playerLocation, OUT playerRotation);

	///Log out every tick 
	/*UE_LOG(LogTemp, Warning, TEXT("PlayerLocation %s, Player Rotation %s")
	,*playerLocation.ToString(),*playerRotation.ToString());*/


	FVector lineTraceEnd = playerLocation + playerRotation.Vector()*reach;
	///Draw a red trace to visualize 
	//DrawDebugLine(GetWorld(), playerLocation, lineTraceEnd, FColor(255, 0, 0),NULL,NULL,NULL,10.f);

	///Setup query parameters
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	///Line-trace (AKA Ray-cast) out to reach distance
	FHitResult lineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT lineTraceHit,
		playerLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams);

	///See what we hit
	if (lineTraceHit.GetActor() != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitting Actor named %s"),
			*lineTraceHit.GetActor()->GetName());
	}
	return lineTraceHit;
}
