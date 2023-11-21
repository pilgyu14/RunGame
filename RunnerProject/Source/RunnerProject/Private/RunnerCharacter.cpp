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
	// �浹 ���� 
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// ī�޶�  ���� 
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->bUsePawnControlRotation = false; // ī�޶� ȸ�� X 

	// �̵��ϴ� �������� ȸ�� 
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);

	// �߷� ���� 
	GetCharacterMovement()->GravityScale = 2.0f; 
	// ĳ���Ͱ� ���߿� ���ִ� ���� ������ �κ� �������� �� ���� �� �� 
	GetCharacterMovement()->AirControl = 0.8f;
	// ���� �ӵ� 
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	// �ٴ� ������
	GetCharacterMovement()->GroundFriction = 3.0f;
	// �̵� �ӵ� 
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	// ���� �ӵ� ? 
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// ���� ĳ���� �ӽ� ��ġ�� 
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
	 
	// ī�޶� ��ġ ������Ʈ 
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

	// �Է� ����
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

		// ���̳� ��ֹ��� �浹�ߴٸ� 
		if (WallSpike || Spike)
		{
			// ���ӿ��� 
			GetMesh()->Deactivate(); 
			GetMesh()->SetVisibility(false); 

			CanMove = false; 

			// ī�޶� ����ũ ���
			auto controller = GetWorld()->GetFirstPlayerController();
			if (controller)
				controller->PlayerCameraManager->StartCameraShake(cameraShake);
			
			//
				// ���� �÷��̾��� ��ġ ��������
			FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

			// ������ ��ƼŬ ����Ʈ�� ��ġ�� ȸ�� ����
			FTransform SpawnTransform(PlayerLocation);

			// ��ƼŬ �ý��� ������Ʈ ����
			UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), dieEffect, SpawnTransform);
			
			// ��ƼŬ ����Ʈ�� �÷��̾ ����ٴϵ��� ����
			ParticleSystemComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);


			FTimerHandle unusedHandle; 
			// 2���Ŀ� RestartLevel 1ȸ ȣ�� 
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


	//  ��ġ ��������
			FVector location = item->GetActorLocation();

			// ������ ��ƼŬ ����Ʈ�� ��ġ�� ȸ�� ����
			FTransform SpawnTransform(location);

			// ��ƼŬ �ý��� ������Ʈ ����
			UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), eatEffect, SpawnTransform);

			item->Destroy2(); 
		}
	}

}

