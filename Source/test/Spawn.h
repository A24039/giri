#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawn.generated.h"

class AStone;

USTRUCT(BlueprintType)
struct FStoneSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float X = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Size = 1.0f;
};

UCLASS()
class TEST_API ASpawn : public AActor
{
	GENERATED_BODY()

public:
	ASpawn();

protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<AActor> BPClass;

public:
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AStone> StoneClass;

	TArray<FStoneSpawnData> SpawnDataList =
	{
		{1000.0f, 1.0f},
		{1200.0f, 0.5f},
		{1400.0f, 2.0f},
		{1600.0f, 1.5f}
	};
};