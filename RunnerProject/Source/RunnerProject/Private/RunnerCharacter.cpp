// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "C:/Unreal/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "C:/Unreal/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include <Blueprint/UserWidget.h>
#include<Blueprint/UserWidgetBlueprint.h>
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spikes.h"
#include "WallSpikes.h"
#include "Item.h"
#include "Engine.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ARunnerCharacter::ARunnerCharacter()
{
	//ScoreWidgetClass = nullptr;
	//ScoreWidgetInstance = nullptr;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hp = maxHp; 
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f); 
	// 충돌 설정 
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// 카메라  생성 
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->bUsePawnControlRotation = false; // 카메라 회전 X 

	// 이동하는 방향으로 회전 
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);

	// 중력 설정 
	GetCharacterMovement()->GravityScale = 2.0f; 
	// 캐릭터가 공중에 떠있는 동안 제어할 부분 높을수록 더 제어 잘 됨 
	GetCharacterMovement()->AirControl = 0.8f;
	// 점프 속도 
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	// 바닥 마찰력
	GetCharacterMovement()->GroundFriction = 3.0f;
	// 이동 속도 
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	// 비행 속도 ? 
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// 게임 캐릭터 임시 위치ㅣ 
	tempPos = GetActorLocation(); 
	zPosition = tempPos.Z + 300.0f; 

}


// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	hud = CreateWidget(GetWorld(), hudFactory);
	hud->AddToViewport();

	GetCapsuleComponent()->OnComponentBeginOverlap.
		AddDynamic(this, &ARunnerCharacter::OnOverlapBegin);

	CanMove = true; 

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	 
	// UI
	//if (ScoreWidgetClass)
	//{
	//	ScoreWidgetInstance = CreateWidget(GetWorld(), ScoreWidgetClass);
	//	if (ScoreWidgetInstance)
	//	{
	//		ScoreWidgetInstance->AddToViewport();
	//	}
	//}
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 
	// 카메라 위치 업데이트 
	tempPos = GetActorLocation(); 
	tempPos.X -= 850.0f; 
	tempPos.Z = zPosition; 
  	camera->SetWorldLocation(tempPos); 
	UpdateUI(); 
}

// Called to bind functionality to input
void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARunnerCharacter::MoveRight);
		EnhancedInputComponent->BindAction(ForwardMoveAction, ETriggerEvent::Triggered, this, &ARunnerCharacter::MoveForward);

		// Slide 
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Triggered, this, &ARunnerCharacter::Slide);

	}

//	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 설정
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//PlayerInputComponent->BindAxis("MoveRight", this, &ARunnerCharacter::MoveRight); 
}

void ARunnerCharacter::MoveRight(const FInputActionValue& Value)
{
	float MovementVector = Value.Get<float>();

	if (CanMove)
	{
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MovementVector);
	}
}

void ARunnerCharacter::MoveForward(const FInputActionValue& Value)
{
	float MovementVector = Value.Get<float>();
	UE_LOG(LogTemp, Error, TEXT("MoveForward"));

	if (CanMove)
	{
		UE_LOG(LogTemp, Error, TEXT("MoveForward@@@"));

		AddMovementInput(FVector(0.0f, 0.0f, 1.0f), MovementVector);
	}
}

void ARunnerCharacter::Slide(const FInputActionValue& Value)
{
	float MovementVector = Value.Get<float>();

	if (CanMove)
	{
		//AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MovementVector);
	}
}

void ARunnerCharacter::RestartLevel()
{
	UGameplayStatics::OpenLevel(this,FName(*GetWorld()->GetName())); 
}


void ARunnerCharacter::UpdateUI()
{

}


void ARunnerCharacter::AddPoint(int32 _point)
{
	point += _point;
}
void ARunnerCharacter::SetPoint(int32 _point)
{
	point = _point;
}


void  ARunnerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		ASpikes* WallSpike = Cast<AWallSpikes>(OtherActor); 
		ASpikes* Spike = Cast<ASpikes>(OtherActor); 
		AItem* item = Cast<AItem>(OtherActor); 

		// 벽이나 장애물에 충돌했다면 
		if (WallSpike || Spike)
		{
			// 게임오버 
			GetMesh()->Deactivate(); 
			GetMesh()->SetVisibility(false); 

			CanMove = false; 

			// 카메라 셰이크 재생
			auto controller = GetWorld()->GetFirstPlayerController();
			if (controller)
				controller->PlayerCameraManager->StartCameraShake(cameraShake);
			
			//
				// 현재 플레이어의 위치 가져오기
			FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

			// 생성할 파티클 이펙트의 위치와 회전 설정
			FTransform SpawnTransform(PlayerLocation);

			// 파티클 시스템 컴포넌트 생성
			UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), dieEffect, SpawnTransform);
			
			// 파티클 이펙트가 플레이어를 따라다니도록 부착
			ParticleSystemComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);


			FTimerHandle unusedHandle; 
			// 2초후에 RestartLevel 1회 호출 
			GetWorldTimerManager().SetTimer(unusedHandle, this,
				&ARunnerCharacter::RestartLevel, 2.f, false); 

			SetPoint(0); 
			hp = maxHp;
		}

		if (item)
		{
			int32 _point = item->GetPoint(); 
			AddPoint(_point);
			UE_LOG(LogTemp, Error, TEXT("Point"));


	//  위치 가져오기
			FVector location = item->GetActorLocation();

			// 생성할 파티클 이펙트의 위치와 회전 설정
			FTransform SpawnTransform(location);

			// 파티클 시스템 컴포넌트 생성
			UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), eatEffect, SpawnTransform);

			item->Destroy2(); 
		}
	}

}

