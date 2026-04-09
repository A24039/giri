#include "MyPlayer.h"
#include "Engine/Engine.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	// CharacterMovementComponentを取得（移動制御の本体）
	UCharacterMovementComponent* Move = GetCharacterMovement();

	// 重力なし
	Move->GravityScale = 0.0f;

	// 飛行モードにする
	Move->SetMovementMode(MOVE_Flying);

	// 最高速度
	Move->MaxFlySpeed = 600.0f;

	// 押したときの加速
	Move->MaxAcceleration = 7000.0f;

	// 離したときの減速
	Move->BrakingDecelerationFlying = 1000.0f;

	// ブレーキ時の摩擦を使う
	Move->bUseSeparateBrakingFriction = true;

	// 少しだけ滑る感じを残す
	Move->BrakingFriction = 1.0f;

	// 摩擦倍率
	Move->BrakingFrictionFactor = 1.0f;



	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			FColor::Green,
			TEXT("Spawnedfffasdfsdaf!")
		);
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EI = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (PressDAction)
		{
			EI->BindAction(PressDAction, ETriggerEvent::Triggered, this, &AMyPlayer::OnPressD);
		}
	}
}

void AMyPlayer::OnPressD(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			0.0f,
			FColor::Yellow,
			FString::Printf(TEXT("AxisValue = %f"), AxisValue)
		);
	}

	AddMovementInput(FVector::RightVector, AxisValue);
}