#include "RoomDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

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
	doorOpensPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void URoomDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentWorldTime = GetWorld()->GetTimeSeconds();

	if (doorPlate && doorPlate->IsOverlappingActor(doorOpensPawn))
	{
		doorLastOpend = currentWorldTime;
		DoorMecanic(DeltaTime, targetRotationYaw);
	}
	else
	{
		if (currentWorldTime - doorLastOpend > doorClosedDelay)
			DoorMecanic(DeltaTime, initialYaw);
	}
}

void URoomDoor::DoorMecanic(float DeltaTime, float rotateTo)
{
	currentRotationYaw = FMath::FInterpConstantTo(currentRotationYaw, rotateTo, DeltaTime, doorSpeed);
	FRotator doorRotation(0.f, currentRotationYaw, 0.f);
	doorRotation.Yaw = currentRotationYaw;
	GetOwner()->SetActorRotation(doorRotation);
}