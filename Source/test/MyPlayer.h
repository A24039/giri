#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

//当たり判定ボックスコンポーネント
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"

//一番最後にする
#include "MyPlayer.generated.h"


UCLASS()
class TEST_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMyPlayer();

	//BPで使えるようにする（当たった相手のタグを調べる）
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void ProcessOverlap(AActor* OtherActor);

	// 判定エリアのコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class UBoxComponent* JudgeArea;

	// 現在のHP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float HP = 50.0f;

	// 最大HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHP = 100.0f;

	// HPの割合（0.0 ~ 1.0）を計算して返す関数
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetHP() const { return HP / MaxHP; }

	//どちらの移動キーを入力しているか
	float moveInput = 0.0f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//キーマッピング
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	//ADの移動キー用のアクション
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* PressDAction;

	// スペースキー用のアクション
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* SpaceAction;
	
	//ADのいどうキーを押したときに呼ぶ関数
	UFUNCTION()
	void OnPress(const FInputActionValue& Value);

	void OnRelease(const FInputActionValue& Value);

	// スペースキーを押したときに呼ぶ関数
	UFUNCTION()
	void OnSpacePressed(const FInputActionValue& Value);

	// スペースを押したら瞬間移動する距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float DashDistance = 200.0f;

	// 色を変えるための関数
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void OnEffectTriggered();

	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// カメラコンポーネントの参照
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* PlayerCamera;

	// FOVの最小値と最大値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxFOV = 120.0f;
};