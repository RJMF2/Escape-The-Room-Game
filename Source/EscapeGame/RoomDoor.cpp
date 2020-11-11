#include "RoomDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

URoomDoor::URoomDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URoomDoor::BeginPlay()
{
	Super::BeginPlay();

	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentRotationYaw = initialYaw;
	targetRotationYaw += initialYaw;
	if (!doorPlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is trying to access the door plate and it's not assigned in the engine"), *GetOwner()->GetName());
	}
	GetAudioComponent();
}

void URoomDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentWorldTime = GetWorld()->GetTimeSeconds();

	if (TotalMassOfActors() >= doorRequierdMass)
	{
		doorLastOpend = currentWorldTime;
		if (!audioComponent)
			return;
		DoorMecanic(DeltaTime, targetRotationYaw);
		if (isDoorOpen)
		{
			audioComponent->Play();
			isDoorOpen = false;
		}
	}
	else
	{
		if (currentWorldTime - doorLastOpend > doorClosedDelay)
		{
			if (!audioComponent)
				return;
			DoorMecanic(DeltaTime, initialYaw);
			if (!isDoorOpen)
			{
				audioComponent->Play();
				isDoorOpen = true;
			}
		}
	}
}

void URoomDoor::DoorMecanic(float DeltaTime, float rotateTo)
{
	currentRotationYaw = FMath::FInterpConstantTo(currentRotationYaw, rotateTo, DeltaTime, doorSpeed);
	FRotator doorRotation(0.f, currentRotationYaw, 0.f);
	doorRotation.Yaw = currentRotationYaw;
	GetOwner()->SetActorRotation(doorRotation);
}

float URoomDoor::TotalMassOfActors()
{
	if (!doorPlate)
		return 0.f;
	totalActorsMass = 0.f;
	TArray<AActor *> overlappingActors;
	doorPlate->GetOverlappingActors(OUT overlappingActors);
	for (AActor *actor : overlappingActors)
	{
		totalActorsMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalActorsMass;
}

void URoomDoor::GetAudioComponent()
{
	audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!audioComponent)
		return;
}
