// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Spawn.generated.h"

// --- 【1】出現情報をまとめる構造体 ---
USTRUCT(BlueprintType)
struct FStoneSpawnData
{
	GENERATED_BODY();

	// スポナーからの相対的な位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocation;

	// 出現させるタイミング（ゲーム開始からの秒数）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime;
};


UCLASS()
class TEST_API AC_Spawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_Spawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//詳細パネルで表示
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Spawn")
	TArray<FStoneSpawnData> SpawnList;

	// 次にスポーンさせるべき要素の番号
	int32 CurrentSpawnIndex = 0;

	// 毎秒チェック用のタイマー関数
	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	void CheckSpawn();

	// 2. BP側で「イベント」として実装したいなら、こちらに ImplementableEvent を付ける
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawning")
	void OnRequestSpawnStone(FVector SpawnLocation);

	// タイマーを制御するためのハンドル
	FTimerHandle SpawnTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
