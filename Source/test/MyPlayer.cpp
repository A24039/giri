#include "MyPlayer.h"
#include "Engine/Engine.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//コンポーネントを実体化させる
	JudgeArea = CreateDefaultSubobject<UBoxComponent>(TEXT("JudgeArea"));

	// ルートコンポーネント
	if (RootComponent)
	{
		JudgeArea->SetupAttachment(RootComponent);
	}

}


void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (!PlayerCamera)
    {
        PlayerCamera = FindComponentByClass<UCameraComponent>();
    }

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

	//キー入力の準備
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

	// HPの割合(0.0 ~ 1.0)を取得[cite: 4]
	float HPRatio = GetHP();

	// HPに応じてFOVを線形補間（Lerp）で計算
	float TargetFOV = FMath::Lerp(MinFOV, MaxFOV, HPRatio);

	// カメラに適用
	if (PlayerCamera)
	{
		PlayerCamera->SetFieldOfView(TargetFOV);
	}
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EI = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (PressDAction)
		{
			// Triggered: 押している間ずっと呼ばれる
			EI->BindAction(PressDAction, ETriggerEvent::Triggered, this, &AMyPlayer::OnPress);

			// Completed: 離した瞬間に呼ばれる（これがないと moveInput が 0 に戻りません）
			EI->BindAction(PressDAction, ETriggerEvent::Completed, this, &AMyPlayer::OnRelease);
		}

		if (SpaceAction)
		{
			EI->BindAction(SpaceAction, ETriggerEvent::Started, this, &AMyPlayer::OnSpacePressed);
		}
	}
}

// キーを離した時に呼ばれる関数（新設）
void AMyPlayer::OnRelease(const FInputActionValue& Value)
{
	moveInput = 0.0f; // 離したら 0 に戻す
}

void AMyPlayer::OnPress(const FInputActionValue& Value)
{
	moveInput = Value.Get<float>();
	AddMovementInput(FVector::RightVector, moveInput);
}

void AMyPlayer::OnSpacePressed(const FInputActionValue& Value)
{

	// 入力方向(moveInput)に合わせて瞬間移動
	if (FMath::Abs(moveInput) > 0.1f)
	{
		FVector DashVector = FVector::RightVector * (moveInput > 0 ? DashDistance : -DashDistance);
		// trueにすると壁を突き抜けずに止まります
		AddActorWorldOffset(DashVector, true);
	}

	// BP側の色替えイベントを呼び出す[cite: 10]
	OnEffectTriggered();

	// JudgeAreaの中に何か重なっているアクターがあるか取得
	TArray<AActor*> OverlappingActors;
	JudgeArea->GetOverlappingActors(OverlappingActors);

	bool bSuccess = false;

	for (AActor* Actor : OverlappingActors)
	{
		// そのアクターが "stone" タグを持っているか判定
		if (Actor && Actor->ActorHasTag(FName("stone")))
		{
			// HPを増やす（最大値を超えないように）
			HP = FMath::Clamp(HP + 10.0f, 0.0f, MaxHP);
			bSuccess = true;
			/*
			// 重複して回復しないように、判定に使ったstoneを破棄するかフラグを立てる
			Actor->Destroy();
			*/
			break;
		}
	}

	if (GEngine)
	{
		if (bSuccess)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("成功！HP回復！"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("空振り！"));
		}
	}
}

void AMyPlayer::ProcessOverlap(AActor* OtherActor)
{
	

	// タグを判定
	if (OtherActor->ActorHasTag(FName("stone")))
	{

		if (!OtherActor || OtherActor == this) return;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				1.0f,
				FColor::Green,
				TEXT("当たった")
			);
		}

		// HPを10減らす
		HP -= 10.0f;

		// 0以下にならないようにクランプ（固定）
		HP = FMath::Clamp(HP, 0.0f, MaxHP);

		OtherActor->Destroy();
		
	}
}