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
	GetPhysicsHandle();
	UserInputActions();
}

void UGrabber::UserInputActions()
{
	if (!physicsHandle)
		return;
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabUnloked);
}

void UGrabber::GetPhysicsHandle()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("please provide the missing component in %s!"), *GetOwner()->GetName());
		return;
	}
}

void UGrabber::Grab()
{
	FHitResult hitRes = GetHitObject();
	FVector lineTraceEnd = GetLineTraceEnd();
	UPrimitiveComponent *componentToGrab = hitRes.GetComponent();

	if (hitRes.GetActor())
	{
		physicsHandle->GrabComponentAtLocation(
			componentToGrab,
			NAME_None,
			GetLineTraceEnd());
	}
}
void UGrabber::GrabUnloked()
{
	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->ReleaseComponent();
	}
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector lineTraceEnd = GetLineTraceEnd();
	if (!physicsHandle)
		return;
	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(lineTraceEnd);
	}
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
