// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Spawn.h"
#include "TimerManager.h"

// Sets default values
AC_Spawn::AC_Spawn()
{
 	//Tick関数を動作させるかどうか
	PrimaryActorTick.bCanEverTick = false;

	SpawnList.Add({ FVector(200.f, 100.f, 0.f), 2.0f });

	SpawnList.Add({ FVector(200.f, 0.f, 0.f), 3.0f });

	SpawnList.Add({ FVector(200.f, 0.f, 0.f), 4.0f });

	SpawnList.Add({ FVector(200.f, 0.f, 0.f), 6.0f });

	SpawnList.Add({ FVector(200.f, 0.f, 0.f), 7.0f });
}

void AC_Spawn::BeginPlay()
{
	Super::BeginPlay();

	CurrentSpawnIndex = 0;
	

	// タイマーをセット
	// 第1引数: ハンドル
	// 第2引数: 実行するオブジェクト (this)
	// 第3引数: 実行する関数のアドレス
	// 第4引数: 間隔（秒）
	// 第5引数: ループするかどうか (true で繰り返す)
	GetWorldTimerManager().SetTimer(SpawnTimerHandle , this, &AC_Spawn::CheckSpawn, 1.0f, true);
}

void AC_Spawn::CheckSpawn()
{
	// ゲーム開始からの経過時間を取得
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// while文に変えることで、同じ秒数（またはそれ以前）のデータをまとめて処理する
	while (CurrentSpawnIndex < SpawnList.Num() && CurrentTime >= SpawnList[CurrentSpawnIndex].SpawnTime)
	{
		// 座標計算
		FVector SpawnLocation = GetActorLocation() + SpawnList[CurrentSpawnIndex].RelativeLocation;

		// BP側に座標を投げる（同時なら同じフレームで複数回呼ばれる）
		OnRequestSpawnStone(SpawnLocation);

		// 次のデータへ進む
		CurrentSpawnIndex++;
	}

	// 全て出し切ったらタイマーを止める
	if (CurrentSpawnIndex >= SpawnList.Num())
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void AC_Spawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
