#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle)
	{
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("please provide the missing component in %s!"), *GetOwner()->GetName());
	}

	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("it exsists!"));
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabUnloked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("it does not exsist"));
	}
}

void UGrabber::Grab()
{
	AActor *hitActor = GetHitObject().GetActor();
	if (hitActor)
		UE_LOG(LogTemp, Warning, TEXT("you Grabing: %s"), *(hitActor->GetName()));
}
void UGrabber::GrabUnloked()
{
	UE_LOG(LogTemp, Warning, TEXT("the grable has released"));
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FHitResult UGrabber::GetHitObject()
{
	FVector lineTraceEnd = GetLineTraceEnd();

	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	FHitResult hitRes;
	FVector playerLocation;
	FRotator playerRotation;
	GetPlayerViewPoint(OUT playerLocation, OUT playerRotation);
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitRes,
		playerLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams);

	return hitRes;
}

FVector UGrabber::GetLineTraceEnd()
{
	// FVector lineDiriction = playerRotation.RotateVector(FVector(100.f, 0.f, 0.f));
	FVector playerLocation;
	FRotator playerRotation;
	GetPlayerViewPoint(OUT playerLocation, OUT playerRotation);
	FVector lineTraceEnd = playerLocation + playerRotation.Vector() * reach;
	return lineTraceEnd;
}
void UGrabber::GetPlayerViewPoint(FVector &playerLocation, FRotator &playerRotation)
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerLocation,
		OUT playerRotation);
}
