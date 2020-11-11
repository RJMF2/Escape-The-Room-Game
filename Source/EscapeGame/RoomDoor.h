// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "RoomDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPEGAME_API URoomDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URoomDoor();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void DoorMecanic(float DeltaTime, float rotateTo);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//methods
	float TotalMassOfActors();
	void GetAudioComponent();

	//variables
	float initialYaw;
	UPROPERTY(EditAnywhere)
	float targetRotationYaw = 90.f;
	float currentRotationYaw;
	UPROPERTY(EditAnywhere)
	ATriggerVolume *doorPlate;
	AActor *doorOpensPawn;
	UPROPERTY(EditAnywhere)
	float doorSpeed = 180;
	float doorLastOpend = 0.f;
	UPROPERTY(EditAnywhere)
	float doorClosedDelay = 2.f;
	float currentWorldTime;
	UPROPERTY(EditAnywhere)
	float doorRequierdMass = 50.f;
	float totalActorsMass;
	UPROPERTY(EditAnyWhere)
	UAudioComponent *audioComponent;
	bool isDoorOpen = false;
};
