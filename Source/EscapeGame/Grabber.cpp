#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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
	UE_LOG(LogTemp, Warning, TEXT("you grabed this"));
}
void UGrabber::GrabUnloked()
{
	UE_LOG(LogTemp, Warning, TEXT("the grable has released"));
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector playerLocation;
	FRotator playerRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerLocation,
		OUT playerRotation);

	// FVector lineDiriction = playerRotation.RotateVector(FVector(100.f, 0.f, 0.f));

	FVector lineTraceEnd = playerLocation + playerRotation.Vector() * reach;
	DrawDebugLine(
		GetWorld(),
		playerLocation,
		lineTraceEnd,
		FColor(0.f, 0.f, 255.f),
		false,
		0.f,
		0,
		5.f);

	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	FHitResult hitRes;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitRes,
		playerLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams);

	AActor *actorHit = hitRes.GetActor();
	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("this is %s"), *(actorHit->GetName()));
	}
}
