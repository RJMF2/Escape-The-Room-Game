#include "WorldPosition.h"
#include "GameFramework/Actor.h"

UWorldPosition::UWorldPosition()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	const FString objectPos = GetOwner()->GetName();

	FString spherePos = GetOwner()->GetActorLocation().ToString();
	//FString test = GetOwner()->GetActorTransform().GetLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is located in %s"), *objectPos, *spherePos);
}

// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
