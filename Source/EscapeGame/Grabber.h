// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPEGAME_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float reach = 150.f;
	UPhysicsHandleComponent *physicsHandle = nullptr;
	UInputComponent *inputComponent = nullptr;

	void GetPhysicsHandle();
	void UserInputActions();
	void Grab();
	void GrabUnloked();
	FHitResult GetHitObject();
	FVector GetLineTraceEnd();
	void GetPlayerViewPoint(FVector &playerLocation, FRotator &playerRotation);
};
