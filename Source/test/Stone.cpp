#include "Stone.h"

AStone::AStone()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStone::BeginPlay()
{
	Super::BeginPlay();
}

void AStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ‘O•ûŒü‚ÉˆÚ“®
	AddActorWorldOffset(-FVector::ForwardVector * Speed * DeltaTime);
}