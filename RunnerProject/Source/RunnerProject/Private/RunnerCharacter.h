// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C:/Unreal/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "RunnerCharacter.generated.h"

UCLASS()
class ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* camera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ForwardMoveAction;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SlideAction;


public:
	UPROPERTY(EditDefaultsOnly, Category = UiData)
		TSubclassOf<class UUserWidget> hudFactory;
	UPROPERTY(EditDefaultsOnly, Category = CameraShake)
		TSubclassOf<class UCameraShakeBase> cameraShake;
	UPROPERTY(EditAnywhere, Category = Effect)
		class UParticleSystem* dieEffect;
	UPROPERTY(EditAnywhere, Category = Effect)
		class UParticleSystem* eatEffect;

public:
	class UUserWidget* hud;
		//public:
		//	UPROPERTY(EditDefaultsOnly, Category = "UI")
		//		TSubclassOf<class UUserWidget> ScoreWidgetClass;
		//private:
		//	UPROPERTY()
		//		class UUserWidget* ScoreWidgetInstance;
public:
	// Sets default values for this character's properties
	ARunnerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void MoveRight(const FInputActionValue& Value);
	void MoveForward(const FInputActionValue& Value);
	void Slide(const FInputActionValue& Value);
public:
	class UCameraComponent* GetCameraComponent() const
	{
		return camera;
	}

	void RestartLevel();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	float zPosition;
	FVector tempPos = FVector();

	bool CanMove;

public:
	UPROPERTY(BlueprintReadOnly, Category = UiData, meta = (AllowPrivateAccess = "true"))
		int32 point;
	UPROPERTY(BlueprintReadOnly, Category = UiData, meta = (AllowPrivateAccess = "true"))
		int32 hp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UiData, meta = (AllowPrivateAccess = "true"))
		int32 maxHp;

protected:
	UFUNCTION()
		void UpdateUI();
public:
	UFUNCTION()
		void AddPoint(int32 _point);
	UFUNCTION()
		void SetPoint(int32 _point);


};
