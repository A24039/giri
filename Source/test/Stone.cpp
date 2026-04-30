#include "Stone.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"

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

    float CurrentSpeed = Speed;

    // プレイヤーを探してHPに応じた倍率をかける
    AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (Player)
    {
        // 基本スピード × (HP/MaxHP の割合) 
        // ※ 0になると止まってしまうので、1.0 + HPRatio などで調整するのもアリです
        CurrentSpeed = Speed * (1.0f + (Player->GetHP()* 2));
    }

    // 計算したスピードで移動[cite: 5]
    AddActorWorldOffset(-FVector::ForwardVector * CurrentSpeed * DeltaTime);
}