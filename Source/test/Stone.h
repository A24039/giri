#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stone.generated.h"

UCLASS()
class TEST_API AStone : public AActor
{
	GENERATED_BODY()

public:
	AStone();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 移動スピード
	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;
};