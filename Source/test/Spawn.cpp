#include "Spawn.h"
#include "Stone.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ASpawn::ASpawn()
{
	PrimaryActorTick.bCanEverTick = false;

	// /Game/Blueprints/BP_MyActor Ç∆Ç¢Ç§BPÇì«Ç›çûÇﬁó·
	static ConstructorHelpers::FClassFinder<AActor> BPClassFinder(
		TEXT("C:/UE/test/Content/stone.uasset")
	);

	if (BPClassFinder.Succeeded())
	{
		BPClass = BPClassFinder.Class;
	}
}

void ASpawn::BeginPlay()
{
	Super::BeginPlay();

	if (!StoneClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("StoneClass is null"));
	}
	else
	{
		for (const FStoneSpawnData& Data : SpawnDataList)
		{
			FVector SpawnLocation(Data.X, 0.0f, 100.0f);

			FActorSpawnParameters Params;
			Params.Owner = this;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AStone* Stone = GetWorld()->SpawnActor<AStone>(
				StoneClass,
				SpawnLocation,
				FRotator::ZeroRotator,
				Params
			);

			if (Stone)
			{
				Stone->SetActorScale3D(FVector(Data.Size));
			}
		}
	}

	if (!BPClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BPClass is null"));
		return;
	}

	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(BPClass, Location, Rotation, Params);

	if (!SpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blueprint spawn failed"));
	}
}